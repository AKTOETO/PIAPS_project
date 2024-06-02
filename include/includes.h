#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/msg.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <pthread.h>
#include <semaphore.h>

// c++
#include <stdexcept>
#include <iostream>
#include <memory>
#include <set>
#include <vector>
#include <functional>
#include <unordered_map>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>

#endif // !INCLUDES_H