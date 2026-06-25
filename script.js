let velocidade = 10;

document.addEventListener("touchmove" , () => {
    velocidade *= 0.85; // reduz a velocidade em 5% a cada movimento
    console.log("velocidade:", velocidade);
});

    const botao = document.getElementById("AimBot");
    const status = document.getElementById("status");

    botao.addEventListener("click", function () {
        status.textContent = "ON";
    
});
