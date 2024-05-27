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

// #include "utils.h"
// #include "constants.h"

// #include "stb_image.h"
// #include "stb_image_write.h"
// #include "stb_image_resize2.h"

#endif // !INCLUDES_H