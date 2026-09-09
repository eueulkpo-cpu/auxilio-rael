document.getElementById("login").addEventListener("click", function() {
    let senha = document.getElementById("senha").value;

    if (senha === "202") {
        document.getElementById("telalogin").style.display = "none";
        document.getElementById("telaprincipal").style.display = "block";
    } else {
        alert("Usuário ou senha incorretos!");
    }
});