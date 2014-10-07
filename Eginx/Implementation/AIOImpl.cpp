#include "AIOimpl"

//return eventfd
int AIOImpl::AIORead(std::string path,void *buf,int epfd=-1)
{
    m_filefd = openFile(path);
    if (-1 == m_filefd) {
        return -1;
    }

    m_ctx = 0;
    if (io_setup(8192, &m_ctx)) {
        perror("io_setup");
        return -1;
    }

    if (posix_memalign(&buf, ALIGN_SIZE, RD_WR_SIZE)) {
        perror("posix_memalign");
        return 5;
    }
    printf("buf: %p\n", buf);

    for (i = 0, iocbp = iocbs; i < NUM_EVENTS; ++i, ++iocbp) {
        iocbps[i] = &iocbp->iocb;
        io_prep_pread(&iocbp->iocb, fd, buf, RD_WR_SIZE, i * RD_WR_SIZE);
        io_set_eventfd(&iocbp->iocb, efd);
        io_set_callback(&iocbp->iocb, aio_callback);
        iocbp->nth_request = i + 1;
    }

    if (io_submit(ctx, NUM_EVENTS, iocbps) != NUM_EVENTS) {
        perror("io_submit");
        return -1;
    }
    return 0;
}

int AIOImpl::AIOWrite(std::string path,char *buf,int epfd=-1)
{
    m_filefd = openFile(path);
    if (-1 == m_filefd) {
        return -1;
    }

    return 0;
}

//featch result
int AIOImpl::onAIO(long &res, long &res2)
{

	uint64_t finished_aio;

	if (read(efd, &finished_aio, sizeof(finished_aio)) != sizeof(finished_aio)) {
		perror("read");
		return -1;
	}

	printf("finished io number: %"PRIu64"\n", finished_aio);

	while (finished_aio > 0) {
		tms.tv_sec = 0;
		tms.tv_nsec = 0;
		r = io_getevents(ctx, 1, NUM_EVENTS, events, &tms);
		if (r > 0) {
			for (j = 0; j < r; ++j) {
				((io_callback_t)(events[j].data))(ctx, events[j].obj, events[j].res, events[j].res2);
			}
			i += r;
			finished_aio -= r;
		}
	}

	return 0;
}




