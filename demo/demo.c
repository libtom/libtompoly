#include <tompoly.h>

void draw_poly(pb_poly *a)
{
   int x;
   char buf[8192];

   if (a->used == 0) {
      printf("0");
   } else {
      for (x = a->used - 1; x >= 0; x--) {
          if (mp_iszero(&(a->terms[x])) == MP_YES) continue;
          mp_toradix(&(a->terms[x]), buf, 10);
          if ((x != a->used - 1) && a->terms[x].sign == MP_ZPOS) {
             printf("+");
          }
          printf(" %sx^%d ", buf, x);
      }
   }
   if (mp_iszero(&(a->characteristic)) == MP_NO) {
      mp_toradix(&(a->characteristic), buf, 10);
      printf(" (mod %s)", buf);
   }
   printf("\n");
}

int main(void)
{
   mp_int chara;
   pb_poly a,b,c,d,e; 
   mp_int  aa,bb,cc,dd,ee;
   int res;

   mp_init(&chara);
   mp_init_multi(&aa,&bb,&cc,&dd,&ee,NULL);
   pb_init_size(&a, &chara, 32);
   pb_init_size(&b, &chara, 32);
   pb_init_size(&c, &chara, 32);
   pb_init_size(&d, &chara, 32);
   pb_init_size(&e, &chara, 32);

   /* a = 3x + 4 */
   mp_set(&(a.terms[1]), 3);
   mp_set(&(a.terms[0]), 4);
   a.used = 2;
   pb_clamp(&a);
   printf("a == \n");
   draw_poly(&a);

   /* b = 7x^2 + 5x + 7 */
   mp_set(&(b.terms[2]), 7);
   mp_set(&(b.terms[1]), 5);
   mp_set(&(b.terms[0]), 7);
   b.used = 3;
   pb_clamp(&b);
   printf("b == \n");
   draw_poly(&b);

   /* c = a + b */
   printf("a + b\n");
   pb_add(&a, &b, &c);
   draw_poly(&c);

   /* c = b + a */
   printf("b + a\n");
   pb_add(&b, &a, &c);
   draw_poly(&c);
  
   /* now test clearing */
   printf("Shifting previous up one\n");
   pb_lshd(&c, 1);
   draw_poly(&c);
   pb_rshd(&c, 1);
   draw_poly(&c);
   pb_lshd(&c, 1);
   pb_add(&a, &b, &c);
   printf("previous add (test if excess cleared)\n");
   draw_poly(&c);

   /* multiply */ 
   printf("Multiply:\n");
   draw_poly(&a);
   draw_poly(&b);
   pb_mul(&a, &b, &c);
   draw_poly(&c);

   /* subtract */
   printf("a - b\n");
   pb_sub(&a, &b, &c);
   draw_poly(&c);
   printf("b - a\n");
   pb_sub(&b, &a, &c);
   draw_poly(&c);


   /* now hijack the char */
   mp_set(&(a.characteristic), 17);
   mp_set(&(b.characteristic), 17);
   mp_set(&(c.characteristic), 17);
   mp_set(&(d.characteristic), 17);
   mp_set(&(e.characteristic), 17);

   /* perform modular addition */
   printf("a + b (in GF(17))\n");
   pb_add(&a, &b, &c);
   draw_poly(&c);
   pb_add(&b, &a, &c);
   draw_poly(&c);

   /* perform modular subtaction */
   printf("a - b (in GF(17))\n");
   pb_sub(&a, &b, &c);
   draw_poly(&c);
   printf("b - a (in GF(17))\n");
   pb_sub(&b, &a, &c);
   draw_poly(&c);

   /* perform division */
   printf("Division (b/a)\n");
   pb_div(&b, &a, &c, &d);
   draw_poly(&a);  
   draw_poly(&b);  
   printf("Q == \n"); draw_poly(&c);  
   printf("R == \n"); draw_poly(&d);  

   /* now test it */
   pb_mul(&a, &c, &c);
   pb_add(&c, &d, &c);
   printf("aQ + R =\n"); draw_poly(&c);

   /* test mod */
   pb_mod(&b, &a, &c);
   printf("b mod a == "); draw_poly(&c);

   /* test GCD of (x^2 - 1) and 5*x^4+5*x^3+7*x^2+8*x+1 [should be x+1] */
   printf("GCD Test\n");
   pb_zero(&a);
   mp_set(&(a.terms[2]), 1);
   mp_set(&(a.terms[0]), 16);
   a.used = 3;
   pb_clamp(&a);
   printf("a == \n");
   draw_poly(&a);

   pb_zero(&b);
   mp_set(&(b.terms[4]), 5);
   mp_set(&(b.terms[3]), 5);
   mp_set(&(b.terms[2]), 7);
   mp_set(&(b.terms[1]), 8);
   mp_set(&(b.terms[0]), 1);
   b.used = 6;
   pb_clamp(&b);
   printf("b == \n");
   draw_poly(&b);

   pb_gcd(&a, &b, &c);
   printf("GCD: "); draw_poly(&c);

   /* test GCD */
   pb_div(&a, &c, &d, &e);
   printf("a/c == "); draw_poly(&d); printf("a mod c == "); draw_poly(&e); pb_mul(&d, &c, &e); printf("should be a: "); draw_poly(&e);
   pb_div(&b, &c, &d, &e);
   printf("b/c == "); draw_poly(&d); printf("b mod c == "); draw_poly(&e); pb_mul(&d, &c, &e); printf("should be b: "); draw_poly(&e);
  
   /* test modular inverse... x^2 + 3 or so should work nice */
   printf("Modular Inverse\n");
   pb_zero(&a);
   mp_set(&(a.terms[2]), 1);
   mp_set(&(a.terms[1]), 0);
   mp_set(&(a.terms[0]), 3);
   a.used = 3;
   pb_clamp(&a);
   printf("a == \n");
   draw_poly(&a);

   /* take inverse of 2x + 9 */
   pb_zero(&b);
   mp_set(&(b.terms[1]), 2);
   mp_set(&(b.terms[0]), 9);
   b.used = 2;
   pb_clamp(&b);
   printf("b == \n");
   draw_poly(&b); 

   /* invert */
   pb_invmod(&b, &a, &c);
   printf("Inverse\n");
   draw_poly(&c);

   /* test */
   pb_mulmod(&b, &c, &a, &d);
   pb_mul(&b, &c, &e);
   printf("This should be 1               : "); draw_poly(&d); 
   printf("This should be equal to k*a + 1: "); draw_poly(&e);

   /* now b has order [dividing] 17^2 - 1 == 288 so b^287 should equal c */
   printf("exptmod test\n");
   mp_set(&aa, 287);
   pb_exptmod(&b, &aa, &a, &d);
   printf("This should be invmod          : "); draw_poly(&d);

   /* test irreduc */
   printf("Irreducibility testing\n");
   pb_isirreduc(&a, &res);
   printf("This should be 1               : %d\n", res);

   pb_isirreduc(&b, &res);
   printf("This should be 0               : %d\n", res);

  

   return EXIT_SUCCESS;
}



