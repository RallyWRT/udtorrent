#ifdef WINDOWS
#include <direct.h>
#include <io.h>
#include <memory.h>
// include windows sha1 header here.
#else
#include <unistd.h>
#include <sys/param.h>
#endif

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>

#include "btconfig.h"
#include "torrent.h"
#include "bencode.h"

#define meta_str(keylist,pstr,pint) decode_query(b,flen,(keylist),(pstr),(pint),(int64_t*) 0,QUERY_STR)
#define meta_int(keylist,pint) decode_query(b,flen,(keylist),(const char**) 0,(pint),(int64_t*) 0,QUERY_INT)
#define meta_pos(keylist) decode_query(b,flen,(keylist),(const char**) 0,(size_t*) 0,(int64_t*) 0,QUERY_POS)


BtTorrent::BtTorrent()
{
	m_announce = (char *) 0;
}

BtTorrent::~BtTorrent()
{
	delete [] m_announce;
}


char* BtTorrent::file2mem(const char *fname, size_t & psiz)
{
  char *b = (char*) 0;
  struct stat sb;
  FILE* fp;
  fp = fopen(fname,"r");
  if( !fp ){
    //CONSOLE.Warning(1, "error, open \"%s\" failed:  %s",fname,strerror(errno));
    return (char*) 0;
  }

  if(stat(fname,&sb) < 0){
    //CONSOLE.Warning(1, "error, stat \"%s\" failed:  %s",fname,strerror(errno));
    return (char*) 0;
  }

  if( sb.st_size > MAX_METAINFO_FILESIZ ){
    //CONSOLE.Warning(1, "error, \"%s\" is really a metainfo file???",fname);
    return (char*) 0;
  }

  b = new char[sb.st_size];
#ifndef WINDOWS
  if( !b ) return (char*) 0;
#endif

  if(fread(b, sb.st_size, 1, fp) != 1){
    if( ferror(fp) ){
      delete []b;
      return (char*) 0;
    }
  }
  fclose(fp);

  psiz = sb.st_size;
  return b;
}


int BtTorrent::test(int argc, char *argv[])
{
#define ERR_RETURN()	{if(b) delete []b; return -1;}

	char *arg_metainfo_file = (char*) 0;
	char *b = (char *) 0;
	int blen=strlen(argv[1]);
	arg_metainfo_file=new char[blen+1];
	arg_metainfo_file[blen]='\0';
	strcpy(arg_metainfo_file,argv[1]);
	printf("file:%s\n",arg_metainfo_file);
	

	//annouce
	size_t flen, q, r;
	const char *s;
	b = file2mem(arg_metainfo_file,flen);
	if( !meta_str("announce",&s,&r) ) ERR_RETURN();
	m_announce = new char [r + 1];
	memcpy(m_announce, s, r);
  	m_announce[r] = '\0';
  	printf("annouce:\t%s\n",m_announce);


	//annouce-list
	if( r = meta_pos("announce-list") ){
	  const char *sptr;
	  size_t slen, n=0;
	  if( q = decode_list(b+r, flen-r, (char *)0) ){
		int alend = r + q;
		r++;  // 'l'
		for( ; r < alend && *(b+r) != 'e' && n < 9; ){	// each list
		  if( !(q = decode_list(b+r, alend-r, (char *)0)) ) break;
		  r++;	// 'l'
		  for( ; r < alend && n < 9; ){  // each value
			if( !(q = buf_str(b+r, alend-r, &sptr, &slen)) )
			  break;  // next list
			r += q;
			if( strncasecmp(m_announce, sptr, slen) ){
			  m_announcelist[n] = new char[slen+1];
			  memcpy(m_announcelist[n], sptr, slen);
			  (m_announcelist[n])[slen] = '\0';
			  n++;
			}
		  }
		  r++;	// 'e'
		}
	  }
	 }
	if( meta_int("creation date", &r) ) m_create_date = (time_t) r;
	if( meta_str("comment", &s, &r) && r ){
	  if( m_comment = new char[r + 1] ){
		memcpy(m_comment, s, r);
		m_comment[r] = '\0';
	  }
	}
	if( meta_str("created by", &s, &r) && r ){
	  if( m_created_by = new char[r + 1] ){
		memcpy(m_created_by, s, r);
		m_created_by[r] = '\0';
	  }
	}
	
	// infohash
	if( !(r = meta_pos("info")) ) ERR_RETURN();
	if( !(q = decode_dict(b + r, flen - r, (char *) 0)) ) ERR_RETURN();




	delete []b;
	
}


