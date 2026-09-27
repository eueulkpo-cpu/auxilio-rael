function controlarEixoY (movimentoY , sensibilidadeY) {
    // Reduzir a intencidade do movimento no eixo Y com base na sensibilidade
    const fatorY = 0.65;

    // Calcular o movimento ajustado no eixo Y
    let movimentoAjustadoY = movimentoY * sensibilidadeY * fatorY;

    //limitar movimento
    const limiteY = 20;

    movimentoAjustadoY = Math.max(-limiteY, Math.min(limiteY, movimentoAjustadoY));
    return movimentoAjustadoY;

return movimentoAjustadoY;
}