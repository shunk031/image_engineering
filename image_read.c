/******************************************************************************
         画像データファイル( lenna.256 )をプログラム中で使用するには
	   
	      画像処理実験では画像データファイルを使用します。以下の
		  ように記述することで、ファイルの内容を配列に格納したり
		  配列に格納されたデータをファイルに書き込むことができます。
	   
******************************************************************************/
#include <stdio.h>
#include <math.h>

#define Nx 256
#define Ny 256

void dft(unsigned char **d_re, unsigned char **d_im, unsigned char s_re[Nx][Ny], unsigned char s_im[Nx][Ny]);
void idft(unsigned char **id_re, unsigned char **id_im, unsigned char d_re[Nx][Ny], unsigned char d_im[Nx][Ny]);

void main(void)
{

    int             i, j;              /** ループ用変数 **/
	unsigned char   f[Nx][Ny];       /** 原画像用の配列 **/
 	unsigned char   g[Nx][Ny];       /** 処理画像用の配列 **/
    FILE            *fp1, *fp2;        /** ファイルポインタ **/

	unsigned char s_re[Nx][Ny];
	unsigned char s_im[Nx][Ny];
	
	unsigned char d_re[Nx][Ny];
	unsigned char d_im[Nx][Ny];
	
	unsigned char id_re[Nx][Ny];
	unsigned char id_im[Nx][Ny];

	unsigned char *p_re[Nx];
	unsigned char *p_im[Nx];
	
	/* lenna.256というファイルを読み込む
	   ためにopenする。rはread（読み込み）を意味している
	*/
	fp1 = fopen( "lenna.256" , "r" );

	/*
	  fp1で定義されたファイルから配列fにデータを読み込む。
	  1は1個のデータの大きさが1バイトであり、
	  65536は1個1バイトのデータが全部で65536個（256x256）
	  あるということを意味している
	*/
	fread( f, 1, 65536, fp1 );
    
	/*
	  fp1 = fopen でファイルを開いたので、
	  読み込み処理が終了したら閉じる
	*/
	fclose( fp1 );

    for( i=0; i<256; i++ ){            /** 配列f[i][j]に格納した画像データ **/
		for( j=0; j<256; j++ ){        /** を、配列g[i][j]に代入している   **/
			g[i][j] = f[i][j];
		}
	}
	
	// 画像のピクセル値を出力
	/* for(i = 0; i < 256; i++ ) { */
	/* 	for(j = 0; j < 256; j++ ) { */
	/* 		printf("%d ", g[i][j]); */
	/* 	} */
	/* 	printf("\n"); */
	/* } */

	for (i = 0; i < Nx; i++) {
		for (j = 0; j < Ny; j++) {
			s_re[i][j] = g[i][j];
			s_im[i][j] = 0;
			d_re[i][j] = 0;
			d_im[i][j] = 0;
		}
	}

	for (i = 0; i < Nx; i++) {
		p_re[i] = d_re[i];
		p_im[i] = d_im[i];
	}
	
	printf("now run dft() ...\n");
	dft(p_re, p_im, s_re, s_im);

	for (i = 0; i < Nx; i++) {
		p_re[i] = id_re[i];
		p_im[i] = id_im[i];
	}
	
	printf("now run idft() ...\n");
	idft(p_re, p_im, d_re, d_im);

	for (i = 0; i < Nx; i++) {
		for (j = 0; j < Ny; j++) {
			g[i][j] = d_re[i][j];
			/* g[i][j] = id_re[i][j]; */
		}
	}
	
	/*
	  処理画像g[i][j]ができたので、それをファイルとして
	  セーブするため、lenna_new.256というファイルを書き込み用として
	  openする。wはwriteを意味し、bはバイナリデータを意味している
	*/
	fp2 = fopen( "lenna_new.256.raw" , "wb" );
 
	/*
	  配列g[i][j]の値をファイルlenna_new.256に書き込む
	*/
	fwrite( g, 1, 65536, fp2 );

	/*
	  f2 = fopen でファイルを開いたので、
	  読み込み処理が終了したら閉じる
	*/
	fclose( fp2 );
}

void dft(unsigned char **d_re, unsigned char **d_im, unsigned char s_re[Nx][Ny], unsigned char s_im[Nx][Ny]) {

	int i, j, x, y, u, v;
	unsigned char temp_re[Nx][Ny] = {0};
	unsigned char temp_im[Nx][Ny] = {0};
	
	// 配列の初期化
	for (i = 0; i < Nx; i++) {
		for (j = 0; j < Ny; j++) {
			temp_re[i][j] = 0;
			temp_im[i][j] = 0;
		}
	}
	
	printf("now run 1dim DFT\n");
	for (y = 0; y < Ny; y++) {
		for (u = 0; u < Nx; u++) {
			for (x = 0; x < Nx; x++) {
				temp_re[y][u] += s_re[y][x] * (cos((2 * M_PI / Nx) * x * u)) + s_im[y][x] * (sin((2 * M_PI / Nx) * x * u));
				temp_im[y][u] += s_re[y][x] * (-sin((2 * M_PI / Nx) * x * u)) + s_im[y][x] * (cos((2 * M_PI / Nx) * x * u));
			}
		}
	}
	
	printf("now run 2dim DFT\n");
	for (u = 0; u < Nx; u++) {
		for (v = 0; v < Ny; v++) {
			for (y = 0; y < Ny; y++) {
				d_re[v][u] += temp_re[y][u] * (cos((2 * M_PI / Ny) * y * v)) + temp_im[y][u] * (sin((2 * M_PI / Ny) * y * v));
				d_im[v][u] += temp_re[y][u] * (-sin((2 * M_PI / Ny) * y * v)) + temp_im[y][u] * (cos((2 * M_PI / Ny) * y * v));
			}
		}
	}

	printf("now dft() finish\n");
	
}

void idft(unsigned char **id_re, unsigned char **id_im, unsigned char d_re[Nx][Ny], unsigned char d_im[Nx][Ny]) {

	int i, j, x, y, u, v;
	unsigned char temp_re[Nx][Ny] = {0};
	unsigned char temp_im[Nx][Ny] = {0};
	
	// 配列の初期化
	for (i = 0; i < Nx; i++) {
		for (j = 0; j < Ny; j++) {
			temp_re[i][j] = 0;
			temp_im[i][j] = 0;
		}
	}
	
	printf("now run 1dim IDFT\n");
	for (y = 0; y < Ny; y++) {
		for (u = 0; u < Nx; u++) {
			for (x = 0; x < Nx; x++) {
				temp_re[y][u] += d_re[y][x] * (cos((-2 * M_PI / Nx) * x * u)) + d_im[y][x] * (sin((-2 * M_PI / Nx) * x * u));
				temp_im[y][u] += d_re[y][x] * (-sin((2 * M_PI / Nx) * x * u)) + d_im[y][x] * (cos((-2 * M_PI / Nx) * x * u));
			}
			temp_re[y][u] /= Nx;
			temp_im[y][u] /= Nx;
		}
	}
	
	printf("now run 2dim IDFT\n");
	for (u = 0; u < Nx; u++) {
		for (v = 0; v < Ny; v++) {
			for (y = 0; y < Ny; y++) {
				id_re[v][u] += temp_re[y][u] * (cos((-2 * M_PI / Ny) * y * v)) + temp_im[y][u] * (sin((-2 * M_PI / Ny) * y * v));
				id_im[v][u] += temp_re[y][u] * (sin((-2 * M_PI / Ny) * y * v)) + temp_im[y][u] * (cos((-2 * M_PI / Ny) * y * v));
			}
			id_re[u][v] /= Ny;
			id_im[u][v] /= Ny;
		}
	}
	printf("now idft() finish\n");
}
