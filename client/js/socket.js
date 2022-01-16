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
        // alert(evt.data);
        if (evt.data > -1) {
            // Cookies.set('username', $('#username').val());
            var username = document.getElementById('username').value;
            document.cookie = "username=" + username;
            document.cookie = "highScore=" + evt.data
            location.href = './game.html'
        }
        else {
            document.getElementById('loginNoti').innerHTML = "account is not available!!!";
        }
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
    else alert("Server error!!!");
}
document.addEventListener("DOMContentLoaded", function (event) {
    if (connected == false) {
        doConnect('ws://localhost:8080');
    }
    /* Connect buttom. */
    // ws.close(); //close connection
    /* Send message. */
    // $("#login").click(function () {
    //     if (connected == true) {
    //         var sendText = $("#username").val();
    //         ws.send(sendText);
    //     }
    // })
    // login
    $('#login').click(function () {
        if ($.fn.validateForm()) {
            if (connected == true) {
                var loginMes = "1_" + $('#username').val() + "_" + $('#password').val();
                sendSocketMessage(loginMes);
            }
            else $('#loginNoti').html('server error!!!');
        }
    })
    $('#signup').click(function () {
        if ($.fn.validateForm()) {
            if (connected == true) {
                var loginMes = "0_" + $('#username').val() + "_" + $('#password').val();
                sendSocketMessage(loginMes);
            }
            else $('#loginNoti').html('server error!!!');
        }
    })
});
function updateDiem(value) {
    var mess = "2_" + getCookie('username') + "_" + value;
    sendSocketMessage(mess);
}