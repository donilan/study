package com.createw;



public class HelloWorld {
    public static void main(String[] argv) {
        java.math.BigDecimal r = new java.math.BigDecimal(1);
        for (long i = 2; i <=50; ++i) {
            r = r.multiply(new java.math.BigDecimal(i));
        }
        System.out.println(r);
    }
}