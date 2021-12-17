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

    if(Cookies.get("highscore")){
        $('#myBestScore').html(Cookies.get("highscore"));
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

function logout() {
    location.href = './login.html';
}

