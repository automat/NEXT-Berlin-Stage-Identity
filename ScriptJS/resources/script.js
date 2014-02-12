/*--------------------------------------------------------------------------*/
// Test instancing
/*--------------------------------------------------------------------------*/

var module = require('anotherModule');

function ContextJS(){
    switch(arguments.length){
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
    }
    
    this.gl = new GL();
    this.time = 0;
    this.frame = 0;
}

ContextJS.prototype.update = function(t,f){
    this.time = t;
    this.frame = f;
};

ContextJS.prototype.draw = function(){
    var gl = this.gl;
    var time = this.time,
        frame = this.frame;
    gl.clear();
    gl.clearColor(0.5 + Math.sin(time) * 0.5,0,1);

    var numi = 50,
    numj = numi;
    var ni,nj;
    var i = -1;
    var j;
    while(++i < numi){
        ni = i / (numi - 1);
        j = -1;
        while(++j < numj){
            nj = j / (numj - 1);
            gl.colorf(ni,nj,0);
            gl.drawCube(-0.5 + nj * 2, 0,-0.5+ ni * 2, 0.0125, 0.0125, 0.0125);
        }
    }
    
    gl.drawVector(0,0,0,-1,1,-1);
    
    gl.colorf(1,1,1,1);
    gl.drawCoordinateFrame();
    gl.drawPlane();
    
    

};


/*--------------------------------------------------------------------------*/
// Test require
/*--------------------------------------------------------------------------*/

/*
// Test if requiring a module, doesnt just copy its source and returns a duplicated instance
var a = require('module');
a.spotted();
a.spotted();
var b = require('module');
a.spotted();
b.spotted();
b.logWaldo();
*/

/*
// Test if rerequiring a deep nested module returns the same object
var a = require('module');
a.spotted();
a.spotted();
var b = require('anotherModule');
b.spotted();
b.report();
*/
 
/*--------------------------------------------------------------------------*/
// Test console
/*--------------------------------------------------------------------------*/

/*
var a = 1,
    c = 1.0,
    d = new Number(1.0),
    e = "abc",
    f = new String("abc"),
    g = true,
    h = new Boolean(true),
    i = {},
    j = new Object(),
    k = [],
    l = new Array(),
    m,
    n = null;

var nested = {
    a : {
        a : new String("abc"),
        b : 1.0
    },

    b : {
        a : new String("def"),
        b : 2.0
    }
}

console.log(this);
*/

/*--------------------------------------------------------------------------*/
// Test scopes
/*--------------------------------------------------------------------------*/

/*
function ClassJS(){
    this.memberA = 1;
    this.memberB = "a";
}

ClassJS.prototype.update = function(){
	//console.log(this.memberA++);

}

ClassJS.prototype.draw = function(){
	console.log("ContextJS draw");
}


function Init(){
    this._classJS  = new ClassJS();
    this._classCpp0 = new ClassCpp();
    this._classCpp0.print();
    this._classCpp1 = new ClassCpp();
    
    this._classCpp0.incr();
    this._classCpp0.add(this._classCpp1);
    
    //console.log(this._classCpp0);
    
    this._classCpp0.print();
    
    console.log(this._classCpp0.getValue());
    
    console.log(this);
}

function MouseDown(){
    console.log('mouse down.');
    this._classCpp0.incr();
    this._classCpp0.print();
}

function Update(secondsElapsed, framesElapsed){
    return;
    console.log('Seconde elapsed: ' + secondsElapsed);
    console.log('Frames elapsed: '  + framesElapsed);

    this._classCpp0.incr();
    this._classCpp0.print();
}
*/
