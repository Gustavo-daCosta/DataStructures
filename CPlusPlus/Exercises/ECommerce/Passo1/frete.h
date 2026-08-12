#ifndef FRETE_H
#define FRETE_H

struct Pedido {
    int index;
    float peso;
};

struct Frete {
    float calcularFreteSedex = [] (const Pedido& pedido) -> float { pedido.peso * ; };
    float calcularFretePAC(const Pedido& pedido) {
        return pedido.peso * 5.0f;
    }
    float calcularRetiradaLocal(const Pedido& pedido) {
        return 0.0f;
    }
};


#endif