#include "AIOimpl"

int AIOImpl::AIORead(std::string path)
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
    m_data.size = RD_WR_SIZE;
    if (posix_memalign(&m_data.buf, ALIGN_SIZE, RD_WR_SIZE)) {
        perror("posix_memalign");
        return 5;
    }
    printf("buf: %p\n", m_data.buf);

	io_prep_pread(&m_iocb.iocb, fd, m_data.buf, RD_WR_SIZE, 0);
	io_set_eventfd(&m_iocb.iocb, efd);
	io_set_callback(&m_iocb.iocb, aio_callback);
	m_iocb.nth_request = 1;

    if (io_submit(ctx, 1, &m_iocbp) != 1) {
        perror("io_submit");
        return -1;
    }
    return 0;
}

int AIOImpl::AIOWrite(std::string path)
{
    m_filefd = openFile(path);
    if (-1 == m_filefd) {
        return -1;
    }

    return 0;
}

//featch result
boost::shared_ptr<AIOINFO_> AIOImpl::onAIO()
{

	uint64_t finished_aio;

	if (read(efd, &finished_aio, sizeof(finished_aio)) != sizeof(finished_aio)) {
		perror("read");
		return -1;
	}

	printf("finished io number: %"PRIu64"\n", finished_aio);

	if (finished_aio > 0) {
		tms.tv_sec = 0;
		tms.tv_nsec = 0;
		r = io_getevents(ctx, 1, 1, &events, &tms);
		if (r > 0) {
			((io_callback_t)(events.data))(ctx, events.obj, events.res, events.res2);
		}

	}

	boost::shared_ptr<AIOINFO_> result(new AIOINFO_);
	result.res = true;
	result.data = m_data;
	return result;
}

