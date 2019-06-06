var state = false;
function toggleLED() {
    if(!state){
        document.getElementById("img").innerHTML ='<img src="led_on.png" alt="LED On" width="150px" height="150">' ;
    }
    else{
        document.getElementById("img").innerHTML ='<img src="led_off.png" alt="LED Off" width="150px" height="150">' ;
    }
    
    state = !state;
    
    var req = false;
    function dataReturned() {
        if (req.readyState == 4) {
            if (req.status == 200) {
                document.getElementById("ledState").innerHTML = "<div>" + req.responseText + "</div>";
            }
        }
    }

    if (window.XMLHttpRequest) {
        req = new XMLHttpRequest();
    }
    else if (window.ActiveXObject) {
        req = new ActiveXObject("Microsoft.XMLHTTP");
    }
    if (req) {
        req.open("GET", "/toggle_led?id=" + Math.random(), true);
        req.onreadystatechange = dataReturned;
        req.send(null);
    }
}

window.onload = function () {

    // doSmth();
    
}


