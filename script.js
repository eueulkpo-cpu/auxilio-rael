let velocidade = 10;

document.addEventListener("touchmove" , () => {
    velocidade *= 0.95; // reduz a velocidade em 5% a cada movimento
    console.log("velocidade:", velocidade);
});
