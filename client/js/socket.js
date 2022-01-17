/* WebSocket. */
var ws;
var connected = false;

/* Establish connection. */
if (connected == false) {
    doConnect('ws://localhost:8080');
}
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
        var response = evt.data;
        console.log(response.split("_"));
        if (response.split("_")[0] == 0) {//đăng ký
            if (response.split("_")[1] == 1) { //đăng ký thành công
                var username = document.getElementById('username').value;
                document.cookie = "username=" + username;
                document.cookie = "highScore=" + 0;
                location.href = './game.html'
            }else if(response.split("_")[1] == 0) {
                document.getElementById('loginNoti').innerHTML = "Registered Account!!!";
            }
            else {
                document.getElementById('loginNoti').innerHTML = "Create account error!!!";
            }
        }
        else if (response.split("_")[0] == 1) {//đăng nhập
            if (response.split("_")[1] != -1) { //đăng nhập thành công
                var username = document.getElementById('username').value;
                document.cookie = "username=" + username;
                document.cookie = "highScore=" + response.split("_")[1];
                location.href = './game.html'
                console.log(1);
            }
            else {
                document.getElementById('loginNoti').innerHTML = "Login error!!!";
            }
        }
        else if (response.split("_")[0] == 2) {//Update điểm
            if (response.split("_")[1] == 1) {//Update điểm
                if (response.split("_")[2] == getCookie('username')) {
                    alert("Chúc mừng bạn vừa giành top 1, hãy giữ vững phong độ nhé!");
                }
                else {
                    alert(response.split("_")[2] + " vừa giành top 1 với " + response.split("_")[3] + " điểm, cướp lại thôi nào!");
                }
            }
        }
        else if (response.split("_")[0] == 3) {//gáy
            if (response.split("_")[1] == getCookie('username')) {
                // alert("Chúc mừng bạn vừa giành top 1, hãy giữ vững phong độ nhé!");
            }
            else {
                alert(response.split("_")[1] + " đang gáy rất to với " + response.split("_")[2] + " điểm");
            }
        }
        else if (response.split("_")[0] == 4) {
            console.log("Kết quả: " + response);
            var top3Rank = response.split("_");
            var j = 2;
            var rank = response.split("_")[1];
            if(rank > 3) {
                document.getElementById("rank").innerHTML = `${rank}`;
            }
            var username = getCookie('username');
            for (var i = 1; i < 4; i++) {
                if (username == top3Rank[j]) {
                    document.getElementById("top" + i + "Name").innerHTML = "you";
                    document.getElementById("top" + i).style.color = 'red';
                    document.getElementById("noRank").style.display = 'none';
                    j++;
                }
                else {
                    document.getElementById("top" + i + "Name").innerHTML = top3Rank[j++];
                }
                document.getElementById("top" + i + "Score").innerHTML = top3Rank[j++];
            }
        }
        else console.log(response);
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
    $('#myBest').click(function () {
        if (connected == true) {
            var mess = "4";
            sendSocketMessage(mess);
        }
        else alert("server error!!!");
    })
    $('#roar').click(function () {
        if (connected == true) {
            var mess = "3_" + getCookie('username') + "_" + getCookie('highScore');
            sendSocketMessage(mess);
        }
        else alert("server error!!!");
    })
});
function updateDiem(value) {
    var score = value.toString()
    var mess = "2_" + getCookie('username') + "_" + score;
    sendSocketMessage(mess);
    // if (connected == true) {
    // }
    // else console.log('server error!!!');
}
// function rank() {
//     var mess = "4";
//     sendSocketMessage(mess);
// }