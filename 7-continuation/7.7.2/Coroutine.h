#pragma once

#include <vector>
#include <functional>
#include <stack>

typedef struct {
    unsigned long eip, esp, ebp, ebx, edi, esi;
    void *stack;
    int length;
} myjmp_buf;

extern int _fastcall mysetjmp(myjmp_buf *jbuf);
extern void _fastcall mylongjmp(myjmp_buf *jbuf, int value);
extern void save_stack(std::vector<char> *dest, unsigned long last, myjmp_buf *callee);

class CoroutineBase {
public:
    virtual ~CoroutineBase();
};

extern std::stack<CoroutineBase *> coroutines;

template <class T> class Coroutine : public CoroutineBase {
    myjmp_buf caller, callee;
    std::vector<char> stack;
    std::function<void()> f;
    int status;
    unsigned long last;

    void exec() {
        f();
        mylongjmp(&caller, 2);
    }

public:
    T value;
    Coroutine() : status(0) {}
    Coroutine(const decltype(f) &f) : f(f), status(0) {}
    void operator=(const decltype(f) &f) { this->f = f; }

    bool operator()() {
        if (status == 0) _alloca(32 * 1024);
        switch (mysetjmp(&caller)) {
        case 1:
            return true;
        case 2:
            coroutines.pop();
            status = 3;
            return false;
        }
        switch (status) {
        case 0:
            last = caller.esp;
            status = 1;
            coroutines.push(this);
            exec();
        case 2:
            if (caller.esp < callee.esp)
                return false;
            status = 1;
            coroutines.push(this);
            mylongjmp(&callee, 1);
        }
        return false;
    }

    T yield(T value) {
        if (coroutines.top() == this) {
            coroutines.pop();
            status = 2;
            this->value = value;
            if (mysetjmp(&callee) == 0) {
                save_stack(&stack, last, &callee);
                mylongjmp(&caller, 1);
            }
        }
        return this->value;
    }
};

template <class T> T yield(T value) {
    auto cr = dynamic_cast<Coroutine<T> *>(coroutines.top());
    return cr ? cr->yield(value) : T();
}
