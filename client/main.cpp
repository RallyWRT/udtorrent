
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "udt.h"
#include "version.h"
#include "peer.h"
#include "torrent.h"
#include "tracker.h"
#include "bencode.h"




int paramCheck(int argc, char *argv[]);
int usage();



int main(int argc ,char *argv[])
{
	UDT::startup();

	// step 1 parase the command line argment
	if(argc < 2 ){
		usage();
		return 1;
	}else if(paramCheck(argc,argv)< 0)return 1;
	// step 2 parase the .torrent file
	// step 3 connect to the tracker
	// step 4 nat for the client
	// step 5 down load data with peer
	// .....


	BtTorrent mBtTorrent;
	mBtTorrent.test(argc,argv);

	
	UDT::cleanup();
	return 0;
}

int paramCheck(int argc, char *argv[])
{

	return 0;
}
int usage()
{
	printf("argment error !\n");
	return 0;
}
	
