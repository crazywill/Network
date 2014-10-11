#ifndef AIOIMPL_H
#define AIOIMPL_H

#include <stdio.h>
#include <errno.h>
#include <libaio.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <boost/shared_ptr.hpp>

#define NUM_EVENTS  128
#define ALIGN_SIZE  512
#define RD_WR_SIZE  1024*NUM_EVENTS

class AIOImpl{
public:
	AIOImpl() :
		m_iocb(new struct custom_iocb),
		m_iocbp(&m_iocb.iocb)
	{

	};

	~AIOImpl(){};

	int initialize(int epfd=-1);

public:
	struct Data{
		size_t size;
		void* buf;
	};
	typedef struct Data DATA_;

	struct AIOInfo{
		long res;
		long res2;
		DATA_ data;
	}AIOINFO_;
	typedef struct AIOInfo ;

public:
	int AIORead(std::string path);
	int AIOWrite(std::string path);
	boost::shared_ptr<AIOINFO_> onAIO();

private:
int openFile(std::string path)
{
    int fd = open(TEST_FILE, O_RDWR | O_CREAT | O_DIRECT, 0644);
    if (filefd == -1) {
        perror("open");
        return -1;
    }
    return  fd;
}

void aio_callback(io_context_t ctx, struct iocb *iocb, long res, long res2)
{
	boost::shared_ptr<AIOINFO_> result(new AIOINFO_);
    struct custom_iocb *iocbp = (struct custom_iocb *)iocb;
    printf("nth_request: %d, request_type: %s, offset: %lld, length: %lu, res: %ld, res2: %ld\n",
            iocbp->nth_request, (iocb->aio_lio_opcode == IO_CMD_PREAD) ? "READ" : "WRITE",
            iocb->u.c.offset, iocb->u.c.nbytes, res, res2);
}

private:
AIORead(const AIORead&);
AIORead& oparetor=(const AIORead&);
int m_epfd;
int m_filefd;
DATA_ m_data;
io_context_t m_ctx;
struct timespec m_tms;
struct io_event m_event;
struct custom_iocb m_iocb;
struct iocb *m_iocbp;
//struct custom_iocb *m_iocbp;
};


#endif
