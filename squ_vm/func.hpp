#ifndef FUNC_HPP
#define FUNC_HPP

class func
{
private:
    int Args;       /* Number of args */
    int Locals;   /* Number of locals */
    int addr;       /* Address */

public:
    func(int, int, int);
    ~func();
    int getArgs();   /* get number of args */
    int getLocals(); /* get number of locals */
    int getAddr();   /* get number of address */
};

func::func(int Args,int Locals,int addr)
{
    this -> Args = Args;
    this -> Locals = Locals;
    this -> addr = addr;
}

int
func::getArgs()
{
    return this -> Args;
}

int
func::getLocals()
{
    return this -> Locals;
}

int
func::getAddr()
{
    return this -> addr;
}

class Context
{
public:
    Context(Context ctx,
            int return_ip,
            func data);
    ~Context();
    Context getCtx();
    int getReturn_ip();
private:
    Context ctx;
    func data;
    int return_ip;
    int* locals; 
};

Context::Context(Context ctx,
                int return_ip,
                func data)
{
    this -> ctx = ctx;
    this -> return_ip = return_ip;
    this -> data = data;
    this -> locals = new int[data.getArgs() +
                            data.getLocals()];
}

Context
getCtx()
{
    return this -> ctx;
}

int
Context::getReturn_ip()
{
    return this -> return_ip;
}

#endif