function ObjA(){
    this.a = "hello";
    this.b = 1.0;
    this.c = 3;
    this.d = true;

    this.e = new Number(1.0);
    this.f = new String("hello");
    this.g = new Boolean(false);

    this.h = {}
    this.i = new Object();
    this.j = new Array(2);
    this.k = [1,2,3,4,5];
}

var a = new ObjA();

const_number = 1.0; //Should not override, cause flagged readonly
console.log(const_number);



console.log(a);