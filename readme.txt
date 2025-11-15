ALLOWED COMMANDS

Everything in C++ 98.
socket, close, setsockopt, getsockname,
getprotobyname, gethostbyname, getaddrinfo,
freeaddrinfo, bind, connect, listen, accept,
htons, htonl, ntohs, ntohl, inet_addr, inet_ntoa,
inet_ntop, send, recv, signal, sigaction,
sigemptyset, sigfillset, sigaddset, sigdelset,
sigismember, lseek, fstat, fcntl, poll (or
equivalent)


FEATURES TO IMPLEMENT

auth, set username, set nickname, join channel, send/receive private messages
user privileges (operator/regular)
commands (operator): kick, invite, topic(change or view)
mode: (i: invite only, t: change topic permission, k: set/change/remove channel passowrd, o: give/take privilege, l: channel user limit)
mode i (yes/no, 1/0), optionnal channel name? or affects current channel
mode t : same(yes/no) same(arg or current)
mode k : newpassword for set, empty for remove OR just "mode k" and then prompt user to enter password and then confirm
mode o : user_to_change, (yes/no)
mode l : n to set limit, empty to remove


each channel has:
	- list of users
	- invite only mode
	- topic can only be changed by operator mode
	- a password (can be empty)
	- a limit (can be empty)

each user has:
	- a username
	- a nickname

create channel???
where to keep track of privilege? each user has a list of channel he is operator in? or each channel has a list of user that are operators in it?
what happens if a user leaves a channel? keep track of privilege or lose it?
who can become operator? who can create a new channel? delete channel? channel-wide privilege vs server-wide privilege

each channel keeps track of active users connected, echo all received lines to all users in that channel


3 classes: server, channel, user (??)

handle encryption, password safety?
handle internet connection or just local on 1 machine?
difference between some client and a bare terminal doing 'nc'?

- ft_split received line
- if first word starts with '/' then process command
- otherwise just send line to channel

use dynamic arrays, make our own ft_realloc in cpp?

degree of freedom regarding commands names and behaviour? what does the client handle/require? test different clients


SOME ANSWERS:

server-wide operator privilege: only one allowed to create a new channel
"join channel_name" creates it if it doesnt already exist
server-wide operator have rights in all channels, each channel has a list of operators that might not have server-wide privilege but have privilege only in this channel
channel deletes if last user leaves it?