#ifndef CGIDATA_HPP
# define CGIDATA_HPP
class CGIData {
public:
	int pipe;
	int pid;

	CGIData(): pipe(0), pid(0) {
	}
};
#endif