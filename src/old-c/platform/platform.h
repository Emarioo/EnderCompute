#pragma once

#include "platform/types.h"


// ##########################
//      Execution Domain
// ##########################

typedef struct {
    const char*  file;
    uint32_t     storage_mb; // UINT32_MAX = 4096 TB
    uint32_t     memory_mb;  // UINT32_MAX = 4096 TB
    const char** accesible_paths;
    // how to provide devices: keyboard 1, mouse 1, keyboard 2, storage device 1 (nvme), storage device 2 (hdd)
    // ui, rendering options
} EDOptions;


typedef uint32_t EDHandle;

EDHandle ed__create(const EDOptions options);

EDOptions ed__options(EDHandle handle);

void ed__update(EDHandle handle, const EDOptions options);

// The signature of 'entry' function:
//    void entry(const char* path, const char* data, int size)
void ed__load(EDHandle handle, const char* path, const char* entry, const char* data, int size);

void ed__destroy(EDHandle handle);




// ##########################
//      File System
// ##########################

typedef uint32_t FSHandle;

#define FS_READ  0x1
#define FS_WRITE 0x2
#define FS_INVALID_HANDLE 0xFFFFFFFF
typedef struct {
    uint64_t file_size;
    bool is_directory;
} FSInfo;

FSHandle fs__open(const char* path, uint32_t flags);
void fs__close(FSHandle handle);

void fs__info(FSHandle handle, FSInfo* info);

uint64_t fs__read(FSHandle handle, uint64_t offset, void* buffer, uint64_t size);
uint64_t fs__write(FSHandle handle, uint64_t offset, void* buffer, uint64_t size);

// @TODO Iterate directory, recursively

// ##########################
//      Memory
// ##########################

// allocate:    ptr = mem_alloc(4096, NULL)
// reallocate:  ptr = mem_alloc(4096, ptr)
// free:        mem_alloc(0, ptr)
void* mem__alloc(uint64_t size, void* old_ptr);
// #define mem__malloc(SIZE) mem__alloc(SIZE, NULL)
// #define mem__realloc(SIZE, PTR) mem__alloc(SIZE, PTR)
// #define mem__free(PTR) mem__alloc(0, PTR)

#define MEM_READ  0x1
#define MEM_WRITE 0x2
#define MEM_EXEC  0x4

void* mem__map(void* address, uint64_t size, int flags);
void  mem__mapflag(void* address, uint64_t size, int flags);
void  mem__unmap(void* address, uint64_t size);




// ##########################
//      Debug/logging
// ##########################

void log__printf(const char* format, ...);


// #############################
//       Threads
// #############################


// #include <stdatomic.h>

typedef struct {
    u64 handle;
    u64 id; // id on windows is 32-bit, on linux id == handle
} Thread;

typedef struct {
    u64 handle; // pthread_mutex_t* on Linux
} Mutex;

typedef struct {
    u64 handle;
} Semaphore;

typedef  u32(*ThreadRoutine)(void*);

void thread__spawn(Thread* thread, ThreadRoutine func, void* arg);
void thread__join(Thread* thread);
bool thread__joinable(Thread* thread);
u64 thread__current_id();

void thread__create_mutex(Mutex* mutex);
void thread__lock_mutex(Mutex* mutex);
void thread__unlock_mutex(Mutex* mutex);
void thread__cleanup_mutex(Mutex* mutex);

void thread__create_semaphore(Semaphore* semaphore, u32 initial, u32 max_locks);
void thread__wait_semaphore(Semaphore* semaphore);
bool thread__signal_semaphore(Semaphore* semaphore, int count);
void thread__cleanup_semaphore(Semaphore* semaphore);

// returns previous value
#define atomic_add(PTR, VAL) __atomic_fetch_add(PTR, VAL, __ATOMIC_SEQ_CST)
// returns previous value
#define atomic_add64(PTR, VAL) __atomic_fetch_add(PTR, VAL, __ATOMIC_SEQ_CST)

// #############################
//       Network
// #############################



typedef enum NetProtocol {
    NET_PROTOCOL_INVALID,
    NET_UDP,
    NET_TCP,
} NetProtocol;

typedef enum _NetFamily {
    NET_FAMILY_INVALID,
    NET_IPV4,
    NET_IPV6,
} _NetFamily;
typedef u8 NetFamily;

typedef struct NetAddress {
    u16       port;
    NetFamily family;
    u8        _reserved;
    union {
        // Stored in network order (big endian)
        u8  address_ipv4[4];
        u8  address_ipv6[16];
    };
} NetAddress;

typedef struct NetSocket {
    NetProtocol protocol;
    u16         local_port;
    NetAddress  remote_address;
    bool        is_connected;

    // Internal
    u64 socked_fd;
} NetSocket;

static void inline net__set_host_address(NetAddress* address) {
    if (address->family == NET_IPV6) {
        memset(address->address_ipv6, 0, sizeof(address->address_ipv6));
        address->address_ipv6[0] = 1;
    } else {
        address->family = NET_IPV4;
        address->address_ipv4[0] = 1;
        address->address_ipv4[1] = 0;
        address->address_ipv4[2] = 0;
        address->address_ipv4[3] = 127;
    }
}

int net__resolve(const char* hostname, NetFamily family, NetAddress* out_address);

int net__bind(NetAddress* address, NetSocket* out_sock);
void net__close(NetSocket* sock);

// TCP client
int net__connect(NetSocket* sock, NetAddress address);

// TCP server
int net__listen(NetSocket* sock, int backlog);
int net__accept(NetSocket* sock, NetSocket* out_sock);

// TCP read/write
u64 net__write(NetSocket* sock, void* buffer, u64 size);
u64 net__read(NetSocket* sock, void* buffer, u64 size);

// UDP send/recieve
u64 net__sendto(NetSocket* sock, NetAddress* address, void* buffer, u64 size);
u64 net__recvfrom(NetSocket* sock, NetAddress* address, void* buffer, u64 size);
