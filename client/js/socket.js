
/* WebSocket. */
var ws;
var connected = false;

/* Establish connection. */
function doConnect(addr) {

    /* Do connection. */
    ws = new WebSocket(addr);

    /* Register events. */
    ws.onopen = function () {
        connected = true;
    };

    /* Deals with messages. */
    ws.onmessage = function (evt) {
        console.log("Recv: " + evt.data + "\n");
    };

    /* Close events. */
    ws.onclose = function (event) {
        console.log("Connection closed: wasClean: " + event.wasClean + ", evCode: "
            + event.code + "\n");
        connected = false;
    };
}
function sendSocketMessage(value) {
    if (connected == true) {
        ws.send(value);
    }
}
document.addEventListener("DOMContentLoaded", function (event) {
    if (connected == false) {
        doConnect('ws://localhost:8080');
    }
    /* Connect buttom. */
    // ws.close(); //close connection
    /* Send message. */
    $("#login").click(function () {
        if (connected == true) {
            var sendText = $("#username").val();
            ws.send(sendText);
        }
    })
});