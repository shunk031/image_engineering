/******************************************************************************
         �摜�f�[�^�t�@�C��( lenna.256 )���v���O�������Ŏg�p����ɂ�
	   
	      �摜���������ł͉摜�f�[�^�t�@�C�����g�p���܂��B�ȉ���
		  �悤�ɋL�q���邱�ƂŁA�t�@�C���̓��e��z��Ɋi�[������
		  �z��Ɋi�[���ꂽ�f�[�^���t�@�C���ɏ������ނ��Ƃ��ł��܂��B
	   
******************************************************************************/
#include <stdio.h>
#include <math.h>

#define Nx 256
#define Ny 256

void dft(unsigned char **d_re, unsigned char **d_im, unsigned char s_re[Nx][Ny], unsigned char s_im[Nx][Ny]);
void idft(unsigned char **id_re, unsigned char **id_im, unsigned char d_re[Nx][Ny], unsigned char d_im[Nx][Ny]);

void main(void)
{

    int             i, j;              /** ���[�v�p�ϐ� **/
	unsigned char   f[Nx][Ny];       /** ���摜�p�̔z�� **/
 	unsigned char   g[Nx][Ny];       /** �����摜�p�̔z�� **/
    FILE            *fp1, *fp2;        /** �t�@�C���|�C���^ **/

	unsigned char s_re[Nx][Ny];
	unsigned char s_im[Nx][Ny];
	
	unsigned char d_re[Nx][Ny];
	unsigned char d_im[Nx][Ny];
	
	unsigned char id_re[Nx][Ny];
	unsigned char id_im[Nx][Ny];

	unsigned char *p_re[Nx];
	unsigned char *p_im[Nx];
	
	/* lenna.256�Ƃ����t�@�C����ǂݍ���
	   ���߂�open����Br��read�i�ǂݍ��݁j���Ӗ����Ă���
	*/
	fp1 = fopen( "lenna.256" , "r" );

	/*
	  fp1�Œ�`���ꂽ�t�@�C������z��f�Ƀf�[�^��ǂݍ��ށB
	  1��1�̃f�[�^�̑傫����1�o�C�g�ł���A
	  65536��1��1�o�C�g�̃f�[�^���S����65536�i256x256�j
	  ����Ƃ������Ƃ��Ӗ����Ă���
	*/
	fread( f, 1, 65536, fp1 );
    
	/*
	  fp1 = fopen �Ńt�@�C�����J�����̂ŁA
	  �ǂݍ��ݏ������I�����������
	*/
	fclose( fp1 );

    for( i=0; i<256; i++ ){            /** �z��f[i][j]�Ɋi�[�����摜�f�[�^ **/
		for( j=0; j<256; j++ ){        /** ���A�z��g[i][j]�ɑ�����Ă���   **/
			g[i][j] = f[i][j];
		}
	}
	
	// �摜�̃s�N�Z���l���o��
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
	  �����摜g[i][j]���ł����̂ŁA������t�@�C���Ƃ���
	  �Z�[�u���邽�߁Alenna_new.256�Ƃ����t�@�C�����������ݗp�Ƃ���
	  open����Bw��write���Ӗ����Ab�̓o�C�i���f�[�^���Ӗ����Ă���
	*/
	fp2 = fopen( "lenna_new.256.raw" , "wb" );
 
	/*
	  �z��g[i][j]�̒l���t�@�C��lenna_new.256�ɏ�������
	*/
	fwrite( g, 1, 65536, fp2 );

	/*
	  f2 = fopen �Ńt�@�C�����J�����̂ŁA
	  �ǂݍ��ݏ������I�����������
	*/
	fclose( fp2 );
}

void dft(unsigned char **d_re, unsigned char **d_im, unsigned char s_re[Nx][Ny], unsigned char s_im[Nx][Ny]) {

	int i, j, x, y, u, v;
	unsigned char temp_re[Nx][Ny] = {0};
	unsigned char temp_im[Nx][Ny] = {0};
	
	// �z��̏�����
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
	
	// �z��̏�����
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
