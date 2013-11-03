#include "rcp_pch.h"
#include "rcp_time.h"

rcp_time_t rcp_time_current(){
	return time(NULL);
}
rcp_string_ref rcp_time_to_string(rcp_time_t time)
{
	size_t bufferSize = 128;
	char buffer[bufferSize];
	struct tm timeinfo;
	localtime_r(&time, &timeinfo);
	strftime(buffer, bufferSize, "%Y-%m-%d-%H-%M-%S", &timeinfo);
	return rcp_string_new(buffer);
}
