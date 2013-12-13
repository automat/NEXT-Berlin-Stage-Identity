var a = 1;

function ClassA(){
    this.a = 10;
    this.b = 10;
}

a = new ClassA();

global_cpp.method();

console.log(global_cpp.x);

global_cpp.x = 16;
global_cpp.logX();

console.log(global_cpp.y);

global_cpp.y = 256;
global_cpp.logY();

console.log(a);