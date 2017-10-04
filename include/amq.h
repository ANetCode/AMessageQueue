#ifndef A_MESSAGE_QUEUE_H
#define A_MESSAGE_QUEUE_H

#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>

namespace amq {

class Log {
public:
    Log(std::string const& filename);
    ~Log();
    template<typename T>
    Log& operator << (T t) {
        ss_mem << t;
        if (fileLog) {
            ss_file << t;
        }
        flush();
        return *this;
    }
    static Log& debug (std::string file, int line);
    static Log& warn (std::string file, int line);
    static Log& error (std::string file, int line);
    static const char* end_line();
private:
    void prelog (std::string file, int line);
    void flush();
private:
    bool fileLog;
    std::ofstream stream_;
    std::stringstream ss_mem;
    std::stringstream ss_file;
    const char* terminal_color;
    const char* log_color;
};

#define LOGD() amq::Log::debug(__FILE__, __LINE__)
#define LOGW() amq::Log::warn(__FILE__, __LINE__)
#define LOGE() amq::Log::error(__FILE__, __LINE__)
#define LOGEND() amq::Log::end_line()

class poll_t;
class context_t;
class context_impl_t;
class protocol_t;
class message_t;

typedef void(*OnProtocolMessage)(message_t);
typedef void(*OnProtocolEvent)(protocol_t*);

class poll_t {
public:
    virtual void Poll() {}
};

class context_t {
public:
    context_t();
    virtual ~context_t();
    void init();
    void addPollItem(poll_t *p);
    void Poll();
    bool validate();
    context_impl_t* GetImpl() { return ptr; }
protected:
    context_impl_t* ptr;
};

class message_t {
public:
    message_t();
    message_t(const message_t &obj);
    message_t(protocol_t*);
    ~message_t();
    void copy(const uint8_t *data, size_t size);
    void copy(const message_t *msg);
    uint8_t* data() { return m_data; }
    size_t   size() { return m_size; }
    void alloc(size_t size);
    void reset();
    void reply(message_t*);
    void set_protocol(protocol_t* p);
    void* __priv_data;
private:
    void init();
    message_t& operator=(const message_t& other) {return *this;};
private:
    uint8_t * m_data;
    size_t m_size;
    size_t m_capacity;
    protocol_t *m_protocol;
};
class protocol_t : public poll_t {
protected:
    protocol_t();
public:
    virtual ~protocol_t() {}
    static protocol_t* Bind(context_t* ctx, std::string uri);
    static protocol_t* InitConnector(context_t *ctx, std::string info);
    virtual void Poll() {}
    virtual bool isAlive() { return false; }
    virtual void recv(message_t*) {}
    virtual void send(message_t*){}
    virtual bool connect(std::string info) { return false;}
    OnProtocolMessage OnMessageCallback;
    OnProtocolEvent OnConnectedCallback;
    OnProtocolEvent OnDisconnectCallback;
    void* data;
    void OnDisconnect();
    int TotalConnections() { return totalConnection; }
protected:
    static protocol_t* CreateProtocol (amq::context_t* ctx, std::string uri);
    virtual bool Bind(std::string info) { return false;}
    context_t *context;
    int        totalConnection;
};

class timer_t {
public:
    static timer_t* create(context_t* context, float duration);
    void* data;
    void OnTimeout();
protected:
    timer_t();
};

class String {
public:

};

bool StringEquals(std::string a, std::string b);
bool StringStartsWith(const std::string content, const std::string prefix);

} // end ns amq

#endif // A_MESSAGE_QUEUE_H