#include <stdio.h>
#include <atmi.h>
#include <fml32.h>	
#include <string.h>
#include "biblioBD02.fml.h"
	
typedef struct biblio{
   int codigo;
   char autor[51];
   char titulo[151];
   char descrp[251];
}biblio;

int main(int argc, char **argv){
   int ivL_resLlamd;
   long lvL_tamLongt;
   float fvL_recValor;
   FBFR32 *fbfr;
   FBFR32 *recv;
   FLDLEN32 flen;
   char msgbuf[64];
   char zaL_bibAutor[51];
   biblio *spL_bibRegis;
   biblio bib;
   int ivL_numOcurr;
   int ivl_iterador;

   printf("*************************************************\n");
   printf("Autor : "); scanf("%s", zaL_bibAutor);
   printf("*************************************************\n");

   /* Conectamos con la aplicacion Tuxedo Server */
   printf ("Conectamos con la aplicacion\n");
   if (tpinit((TPINIT *) NULL) == -1){
       printf("Error en la conexion, tperrno = %d \n", tperrno);
       return (1);
   }

   // Reservamos espacio para el buffer FML
   printf ("Reservamos Espacio para los buffers FML\n");
   if ((fbfr = (FBFR32 *) tpalloc("FML32", NULL, 1024)) == NULL){
       printf("Error Reservando espacio para Buffer fbfr\n");
       tpterm();
       return(1);
   }
   if ((recv = (FBFR32 *) tpalloc("FML32", NULL, 1024)) == NULL){
       printf("ErrorReservando espacio para Buffer recv\n");
       tpterm();
       return(1);
   }
   /* Manejo del Buffer FML */
   if(Fadd32 (fbfr, AUTOR, zaL_bibAutor, 0) < 0) {
      printf ("\n\tError insertando campo FML (AUTOR)");
      tpfree((char*)fbfr);
      tpterm();
      return (0);
   }
   // Invocamos el servicio 
   printf("Llamada al servicio 'SELECT_FML32'\n");
   if(tpcall("SELECT_FML32", (char *)fbfr, 0, (char **)&recv, &lvL_tamLongt, 0L)==-1){
      printf("Error en la llamada al servicio: tperrno = %d\n", tperrno);
      tpfree((char *)fbfr);
      tpfree((char *)recv);
      tpterm();
      return (1);
   }
   if((ivL_numOcurr = Foccur32(recv, CODIGO)) < 0) {
      printf("Error en Foccur32\n");
      tpfree((char *)fbfr);
      tpfree((char *)recv);
      tpterm();
      return (1); 
   }
   printf("Numero de ocurrencias: %d\n", ivL_numOcurr);
   spL_bibRegis = (biblio *) malloc(sizeof(biblio) * ivL_numOcurr);

  flen = sizeof(msgbuf);
  printf("Respuesta del servidor para el autor: %s\n", zaL_bibAutor);
  printf("***************************************************\n");
  for(ivl_iterador = 0;ivl_iterador < ivL_numOcurr; ivl_iterador++) {
      printf("Numero de registro: %d\n", (ivl_iterador + 1));
      Fget32(recv, CODIGO, ivl_iterador, (char *)&spL_bibRegis[ivl_iterador].codigo,0);
      printf("Codigo del libro : %d\n", spL_bibRegis[ivl_iterador].codigo);
      Fget32(recv, TITULO, ivl_iterador, (char *)spL_bibRegis[ivl_iterador].autor,0);
      printf("Titulo del libro : %s\n", spL_bibRegis[ivl_iterador].autor);
      Fget32(recv, DESC, ivl_iterador, (char *)spL_bibRegis[ivl_iterador].descrp,0);
      printf("Descripcion : %s\n", spL_bibRegis[ivl_iterador].descrp);
      }
   // Liberamos el buffer y desconectamos de la aplicacion
   printf("Liberamos Buffer y desconectamos de la aplicacion\n");
   tpfree((char *)fbfr);
   tpfree((char *)recv);
   tpterm();
   return 0;
}
    

























