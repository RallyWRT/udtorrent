#ifndef __TORRENT_H_
#define __TORRENT_H_

#include <inttypes.h>
#include <sys/types.h>


//Representation  of .torrent
class BtTorrent
{
public:
	char *m_announce; //the tracker's url	
	char *m_announcelist[9];// the annouce-list opt
	
	char *m_comment;// comment opt
	char *m_created_by;// created by  opt

	time_t m_create_date;//create-date opt
	BtTorrent();
	~BtTorrent();
	int test(int argc, char *argv[]);
	char* file2mem(const char *fname, size_t & psiz);
};

#endif
