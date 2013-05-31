
#ifndef __HELLO_H__
#define __HELLO_H__

class Hello {
    private:
        static const char * const message;
    public:
        static const char * const get_message(void);
};

const char * const Hello::message = "Hello World!";

const char * const Hello::get_message(void) {
    return message;
}

#endif // __HELLO_H__
