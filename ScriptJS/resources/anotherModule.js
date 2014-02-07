var privateMethod = function(){
    console.log("Where is Waldo?");
}

var waldoSpotted = 0;

var b = {
    add : function(a,b) {
        return a + b;
    },
    
    expose : function(){
        privateMethod();
    },
    
    spotted : function(){
        waldoSpotted++;
    },
    
    hidden : function(){
        if(waldoSpotted == 0){
            return;
        }
            waldoSpotted--;
    },
    
    report : function(){
        console.log("Found Waldo " + waldoSpotted + " times!");
    }
}

module.exports = b;