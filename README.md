Overview:
Dynamic Bandwidth Limiter can control the bandwidth of the individual applications based on the inputs from the user.

Features:
1.It can limit the download rate of a application. The user enters the rate at which he wants to download in kbps.
2.Multiple downloads can be done simultaneously.
3.The download can be paused and  a new limiting rate can be used for continuing the download.

Environment required:
Linux environment with g++ installed.

Usage:
1.Copy bandwidthlimiter.cpp and makefile into a directory.
2.Enter make in the directory in command line.
3.Normal download with rate limiting parameter
./bandwidthlimiter <URL> <limit-rate in kbps>
Eg: ./bandwidthlimiter www.youtube.com 100
This download youtube.com homepage with a speed of 100kbps
4.Multiple downloads simultaneously.
./bandwidthlimiter <URL1> <limit-rate in kbps> & ./bandwidthlimiter <URL2> <limit-rate in kbps> & ...
Eg: ./bandwidthlimiter www.youtube.com 100 & ./bandwidthlimiter www.gmail.com 10
5.Pausing download and continuing.
Eg: Initially youtube.com downloaded at 100 kbps
>>./bandwidthlimiter www.youtube.com 100
..
..
..
..
<Press Ctrl+c to pause download>
>>./bandwidthlimiter www.youtube.com 10
..
..
..
Complete
The download is continued now at 10 kbps.
