#include "cpptqdm.h"
#include<unistd.h>
int main() {
	ProgressBar* pbar = new ProgressBar(10);
	for ( int i = 0; i < 10; i++ ) {
		pbar->countUp();
		sleep(1);
	}
	delete pbar;
	return 0;
}