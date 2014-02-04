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
    this._classJS = new ClassJS();
}

function Update(secondsElapsed, framesElapsed){
    console.log('Seconde elapsed: ' + secondsElapsed);
    console.log('Frames elapsed: '  + framesElapsed);

    this._classJS.update();
}








