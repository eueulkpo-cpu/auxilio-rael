function amortecerComLimete(delta, limite = 0.05 ) {
    return  (delta * limite) / (Math.abs(delta) + limite);
}