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

#define NUM_EVENTS  128
#define ALIGN_SIZE  512
#define RD_WR_SIZE  1024

class AIOImpl{
public:
AIOImpl(){};
~AIOImpl(){};
int initialize(int epfd=-1);

//return eventfd
int AIORead(std::string path,void *buf);

int AIOWrite(std::string path,char *buf);

//featch result
void onAIO(long &res, long &res2);

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
    struct custom_iocb *iocbp = (struct custom_iocb *)iocb;
    printf("nth_request: %d, request_type: %s, offset: %lld, length: %lu, res: %ld, res2: %ld\n",
            iocbp->nth_request, (iocb->aio_lio_opcode == IO_CMD_PREAD) ? "READ" : "WRITE",
            iocb->u.c.offset, iocb->u.c.nbytes, res, res2);
   m_res = res;
   m_res2 = res2;
}

private:
AIORead(const AIORead&);
AIORead& oparetor=(const AIORead&);
int m_epfd;
int m_filefd;
io_context_t m_ctx;
struct timespec m_tms;
struct io_event m_events[NUM_EVENTS];
struct custom_iocb m_iocbs[NUM_EVENTS];
struct iocb *m_iocbps[NUM_EVENTS];
struct custom_iocb *m_iocbp;
long m_res;
long m_res2;
};


#endif
