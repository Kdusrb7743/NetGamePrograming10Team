#include "SNetworkManager.h"

int main(int argc, char* argv[])
{
	SNetworkManager NM;
	NM.init();

	while (NM.m_clientCnt < 3) 
	{
		NM.acceptClient();
	}

	NM.releaseServer();
	return 0;
}
