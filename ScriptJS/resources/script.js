
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

var a = new ClassCpp();

a.add("hello");

//console.log(this);

//new Class();

//console.log(this);







