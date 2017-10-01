#ifndef A_MESSAGE_QUEUE_H
#define A_MESSAGE_QUEUE_H

#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>

namespace amq {

class Log {
public:
    Log(std::string const& filename);
    ~Log();
    template<typename T>
    Log& operator<<(T t) {
        std::cout << t;
        if (fileLog) {
            stream_ << t;
        }
        return *this;
    }
    static Log& debug();
private:
    bool fileLog;
    std::ofstream stream_;
};

#define LOGD() amq::Log::debug() << __FILE__ << ": " << __LINE__ << " "
#define LOGEND() "\n";

class context_t;
class protocol_t;
class message_t;

typedef void(*OnProtocolMessage)(message_t);

class context_t {
public:
    context_t();
    void AddProtocol(protocol_t *p);
    void Poll();
private:
    std::list<protocol_t*> protocols;
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
    void send(std::string uri);
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
class protocol_t {
protected:
    protocol_t();
public:
    virtual ~protocol_t() {}
    static protocol_t* Bind(context_t* ctx, std::string uri);
    static protocol_t* InitConnector(context_t *ctx, std::string info);
    virtual void Poll() {}
    bool hasRequest() { return bHasRequest; }
    virtual bool isAlive() { return false; }
    virtual void recv(message_t*) {}
    virtual void send(message_t*){}
    virtual bool connect(std::string info) { LOGD()<< "xxxx" << LOGEND(); return false;}
    OnProtocolMessage OnMessageCallback;
    void* data;
protected:
    static protocol_t* CreateProtocol (amq::context_t* ctx, std::string uri);
    virtual bool Bind(std::string info) { return false;}
    context_t *context;
    bool       bHasRequest;
};

class String {
public:

};

bool StringEquals(std::string a, std::string b);

} // end ns amq

#endif // A_MESSAGE_QUEUE_H