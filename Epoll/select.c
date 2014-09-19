asmlinkage long sys_select(int n, fd_set __user *inp, fd_set __user *outp,
            fd_set __user *exp, struct timeval __user *tvp)
{
    struct timespec end_time, *to = NULL;
    struct timeval tv;
    int ret;

    if (tvp) {
        if (copy_from_user(&tv, tvp, sizeof(tv)))
            return -EFAULT;

        to = &end_time;
        if (poll_select_set_timeout(to,
                tv.tv_sec + (tv.tv_usec / USEC_PER_SEC),
                (tv.tv_usec % USEC_PER_SEC) * NSEC_PER_USEC))
            return -EINVAL;
    }

    ret = core_sys_select(n, inp, outp, exp, to);
    ret = poll_select_copy_remaining(&end_time, tvp, 1, ret);

    return ret;
} 
//前面是从用户控件拷贝各个fd_set到内核空间，接下来的具体工作在core_sys_select中，
//core_sys_select->do_select,真正的核心内容在do_select里：
int do_select(int n, fd_set_bits *fds, struct timespec *end_time)
{
    ktime_t expire, *to = NULL;
    struct poll_wqueues table;
    poll_table *wait;
    int retval, i, timed_out = 0;
    unsigned long slack = 0;

    rcu_read_lock();
    retval = max_select_fd(n, fds);
    rcu_read_unlock();

    if (retval < 0)
        return retval;
    n = retval;

    poll_initwait(&table);
    wait = &table.pt;
    if (end_time && !end_time->tv_sec && !end_time->tv_nsec) {
        wait = NULL;
        timed_out = 1;
    }

    if (end_time && !timed_out)
        slack = estimate_accuracy(end_time);

    retval = 0;
    for (;;) {
        unsigned long *rinp, *routp, *rexp, *inp, *outp, *exp;

        set_current_state(TASK_INTERRUPTIBLE);

        inp = fds->in; outp = fds->out; exp = fds->ex;
        rinp = fds->res_in; routp = fds->res_out; rexp = fds->res_ex;

        for (i = 0; i < n; ++rinp, ++routp, ++rexp) {
            unsigned long in, out, ex, all_bits, bit = 1, mask, j;
            unsigned long res_in = 0, res_out = 0, res_ex = 0;
            const struct file_operations *f_op = NULL;
            struct file *file = NULL;

            in = *inp++; out = *outp++; ex = *exp++;
            all_bits = in | out | ex;
            if (all_bits == 0) {
                i += __NFDBITS;
                continue;
            }

            for (j = 0; j < __NFDBITS; ++j, ++i, bit <<= 1) {
                int fput_needed;
                if (i >= n)
                    break;
                if (!(bit & all_bits))
                    continue;
                file = fget_light(i, &fput_needed);
                if (file) {
                    f_op = file->f_op;
                    mask = DEFAULT_POLLMASK;
                    if (f_op && f_op->poll)
                        mask = (*f_op->poll)(file, retval ? NULL : wait);
                    fput_light(file, fput_needed);
                    if ((mask & POLLIN_SET) && (in & bit)) {
                        res_in |= bit;
                        retval++;
                    }
                    if ((mask & POLLOUT_SET) && (out & bit)) {
                        res_out |= bit;
                        retval++;
                    }
                    if ((mask & POLLEX_SET) && (ex & bit)) {
                        res_ex |= bit;
                        retval++;
                    }
                }
            }
            if (res_in)
                *rinp = res_in;
            if (res_out)
                *routp = res_out;
            if (res_ex)
                *rexp = res_ex;
            cond_resched();
        }
        wait = NULL;
        if (retval || timed_out || signal_pending(current))
            break;
        if (table.error) {
            retval = table.error;
            break;
        }

        /*
         * If this is the first loop and we have a timeout
         * given, then we convert to ktime_t and set the to
         * pointer to the expiry value.
         */
        if (end_time && !to) {
            expire = timespec_to_ktime(*end_time);
            to = &expire;
        }

        if (!schedule_hrtimeout_range(to, slack, HRTIMER_MODE_ABS))
            timed_out = 1;
    }
    __set_current_state(TASK_RUNNING);

    poll_freewait(&table);

    return retval;
} 
