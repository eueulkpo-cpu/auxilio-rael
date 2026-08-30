float limitar_sensibilidade(float valor_atual, float limitee_maximo) {
    if (valor_atual > limitee_maximo ) {
        return limitee_maximo;
    }

   float sensibilidade = 00.05f;
   float limete = 00.06f;
   return valor_atual + sensibilidade * (limitee_maximo - valor_atual) * limete;
}