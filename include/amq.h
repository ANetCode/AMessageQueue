#ifndef A_MESSAGE_QUEUE_H
#define A_MESSAGE_QUEUE_H

#include <string>
#include <list>
#include <vector>

namespace amq {
    class debug{
    public:
        static std::vector<std::string> GetStackTrace();
        static void PrintfStackTrace();
    };
}

namespace amq {

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
    virtual void recv(message_t*) {}
    virtual void send(message_t*){}
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