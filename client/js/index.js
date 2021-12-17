document.addEventListener("DOMContentLoaded", function () {
    $('#game-screen').hide();
    $("#mybest-screen").show();
    $('#menu').hide('slow');
    $("#playGame").on("click", function () {
        $('#game-screen').show('slow');
        $('.runner-canvas').addClass('show-canvas');
        $('#menu').hide('slow');
        $("#mybest-screen").hide('slow');
    });
    $(".back-menu").on("click", function () {
        $('#game-screen').hide('slow');
        $('.runner-canvas').removeClass('show-canvas');
        $('#menu').show('slow');
        $("#mybest-screen").hide('slow');
    });
    $("#myBest").on("click", function () {
        $("#mybest-screen").show('slow');
        $('#game-screen').hide('slow');
        $('.runner-canvas').removeClass('show-canvas');
        $('#menu').hide('slow');
    });


    //get my Score
    //check login
    if(location.pathname == '/client/game.html'){
        checkLogin();
        $('#myBestScore').html(getCookie('highScore'));
    }
    //validate
    $.fn.validateForm = function () {
        if ($('#username').val() == '' && $('#password').val() == '') {
            $('#loginNoti').html('Please enter username and password!!!');
        }
        else
            if ($('#username').val() == '') {
                $('#loginNoti').html('Please enter username!!!');
            }
            else
                if ($('#password').val() == '') {
                    $('#loginNoti').html('Please enter password!!!');
                }
                else {
                    $('#loginNoti').html('');
                    return true;
                }
    }
    //for login

});
function setCookie(cname, cvalue, exdays) {
    const d = new Date();
    d.setTime(d.getTime() + (exdays * 24 * 60 * 60 * 1000));
    let expires = "expires=" + d.toGMTString();
    document.cookie = cname + "=" + cvalue + ";" + expires + ";path=/";
}

function getCookie(cname) {
    let name = cname + "=";
    let decodedCookie = decodeURIComponent(document.cookie);
    let ca = decodedCookie.split(';');
    for (let i = 0; i < ca.length; i++) {
        let c = ca[i];
        while (c.charAt(0) == ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0) {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}

function checkLogin() {
    let user = getCookie("username");
    if (user=='') {
        location.href = './login.html'
    }
}

function logout() {
    location.href = './login.html';
}

