#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <random>
#include <string.h>
/**
	Constantes
**/
#define w 500 		// Tamaño de ventana
#define minX 14		// Valor minimo del eje X para el sector del snake
#define minY 34		// Valor minimo del eje Y para el sector del snake
#define maxX 490	// Valor maximo del eje X para el sector del snake
#define maxY 490	// Valor maximo del eje Y para el sector del snake
#define LEFT 97		// Valor de izquierda del teclado (ascii de 'a')
#define RIGHT 100	// Valor de derecha del teclado (ascii de 'd')
#define UP 119		// Valor de arriba del teclado (ascii de 'w')
#define DOWN 115	// Valor de abajo del teclado (ascii de 's')
#define ESC 27		// Valor de ESC del teclado (ascii de 'ESC')
using namespace cv;	// Usando el namespace
/**
	Funciones
**/
void MyLine(Mat img, Point start, Point end);	// Hace una linea en la imagen pasada por parámetro, arrancando en el punto start hasta el punto end
void MyText(Mat img, Point pt, char str[]);		// Escribe la string pasada por parámetro en la imagen pasada por parámetro, tomando el punto como referencia la base izquierda del texto
void printTitle(Mat img);						// Imprime el titulo en la imagen pasada por parámetro (el cuadrado, el titulo de score, etc)
void printScore(Mat img, int score);			// Imprime el score pasado por parámetro
void printLives(Mat img, int lives);			// Imprime las vidas pasadas por parámetro
void intToString(char *str, int n);				// Transforma un número entero en una string equivalente
int digits(int num);							// Entrega la cantidad de dígitos de un número
int randomNum(int max, int min);				// Entrega un número aleatorio dentro del rango pasado por parámetro (utiliza la librería random de c++)
Point setFruit();								// Entrega el punto en el que va a ser seteado la fruta
void printFruit(Mat img, Point c);				// Imprime la fruta en la imagen pasada por parámetro en el punto establecido
/**
	Clase Snake
**/
class Snake {
	Point c[500]; 					// Array de valores de coordenadas
	int direction;					// Dirección a la que está yendo el snake
	int len;						// Largo actual
public:
	Snake(int);						// Funcion construct, inicializa el snake con la cantidad de bolitas pasadas por parámetro
	void move(int newDir);			// Mueve el snake hacia la dirección pasada por parámetro
	void enlarge();					// Alarga el snake en 1
	void print(Mat img);			// Imprime el snake en la imagen pasada por parámetro
	bool contactF(Point p);			// Devuelve si el snake hace contacto con el punto pasado por parámetro
	bool contactS();				// Devuelve si el snake hace contacto con si mismo
};
/**
	Main
**/
int main() {
	char window[6] = "Snake"; 				// Defino el nombre de la ventana
	Mat image = Mat::zeros(w, w, CV_8UC3);	// Inicializo la imagen principal, y la pongo en color negro
	Snake sk(8);							// Inicializo el snake con 8 unidades
	Point fruit = setFruit();				// Seteo el punto aleatorio de la fruta
	int score = 0;							// Inicializo el score en 0
	int lives = 3;							// Inicializo las vidas en 3
	while (true) {							// Loop principal
		image = Mat::zeros(w, w, CV_8UC3);	// 		Limpio la imagen en color negro
		printTitle(image);					// 		Imprimo el titulo en la imagen
		if (sk.contactF(fruit)) {			// 		Si el snake hace contacto con la fruta
			sk.enlarge();					// 			La alargo en 1
			fruit = setFruit();				//			Seteo otro punto aleatorio en la fruta
			score += 100;					//			Le sumo 100 al score
		}									//
		if (sk.contactS()) {				//		Si el snake hace contacto con si mismo
			Snake nSk(8);					//			Inicializo un nuevo snake con 8 unidades
			sk = nSk;						//			Sobreescribo el snake viejo con el nuevo
			lives--;						//			Resto una vida
			if (score > 500) {				//			Si el score es mayor a 500
				score = score - 500;		//				Le resto 500
			}								//
			else {							//			Sino
				score = 0;					//				Seteo el score en 0
			}								//
		}									//
		if (lives==0) {						// 		Si las vidas son 0
			exit(0);						//			Cierra el programa
			break;							//			Rompo el loop
		}									//
		sk.print(image);					//		Imprimo el snake en la imagen
		printScore(image,score);			//		Imprimo el score
		printLives(image, lives);			// 		Imprimo las vidas
		printFruit(image,fruit);			//		Imprimo la fruta
		imshow(window, image);				//		Muestro la imagen en la ventana
		int dir = waitKey(0);				//		Espero el teclado, y cargo la dirección (similar a getch)
		sk.move(dir);						//		Muevo el snake al a dirección nueva
	}										//
	return(0);								// Termino el programa
}
/**
	Funciones
**/
void printScore(Mat img, int score) {
	char* str = new char[digits(score) + 1];	// Genero un string en la memoria dinámica con el largo de la cantidad de dígitos del score
	intToString(str, score);					// Cargo el string con el valor del número
	int scoreStart = w-150;						// Seteo la posición de X del punto, que es donde empieza el bloque de score
	MyText(img, Point(scoreStart+70,17), str);	// Imprimo el string en la imagen, en el punto definido anteriormente, desfazado en 70 pixels y a la altura 17
	delete str;									// Libero el string de la memoria dinámica
}
void printLives(Mat img, int lives) {
	char* str = new char[digits(lives) + 1];		// Genero un string en la memoria dinámica con el largo de la cantidad de dígitos de las vidas
	intToString(str, lives);						// Cargo el string con el valor del número
	int scoreStart = w - 250;						// Seteo la posición de X del punto, que es donde empieza el bloque de las vidas
	MyText(img, Point(scoreStart + 50, 17), str);	// Imprimo el string en la imagen, en el punto definido anteriormente, desfazado en 50 pixels y a la altura 17
	delete str;										// Libero el string de la memoria dinámica
}
void printTitle(Mat img) {
	Scalar bkColor = Scalar(90, 90, 90);							// Seteo la variable de color de fondo (Blue=90, Green=90, Red=90), Gris
	Scalar DrawColor = Scalar(30, 30, 30);							// Seteo la variable de color de linea (Blue=30, Green=30, Red=30), Gris oscuro
	rectangle(img,Point(0,0),Point(w,w),bkColor,-1, 8);				// Dibujo el rectángulo de fondo (imagen, punto inicial(superior izquierdo), punto final(inferior derecho), color, Grosor en pixels(-1 significa lleno), tipo de linea 8 (generalmente siempre se pone 8))
	rectangle(img,Point(2, 2),Point(w - 3, w - 3),DrawColor,1,8);	// "
	rectangle(img,Point(2, 22),Point(w - 3, w - 3),DrawColor,1,8);	// "
	int scoreStart = w - 150;										// Inicio del bloque de score (eje X)
	int livesStart = w - 250;										// Inicio del bloque de vidas (eje X)										
	int titleStart = 3;												// Inicio del bloque de titulo (eje X)
	char titleText[] = "Snake";										// Titulo del titulo
	char livesText[] = "Lives";										// Titulo de las vidas
	char scoreText[] = "Score";										// Titulo del score
	MyText(img, Point(titleStart, 17), titleText);					// Escribo el texto: (imagen, punto de referencia (inferior izquierdo), string a escribir)
	MyText(img, Point(scoreStart, 17), scoreText);					// "
	MyText(img, Point(livesStart, 17), livesText);					// "
	MyLine(img, Point(scoreStart - 2, 2), Point(scoreStart - 4, 22));	// Dibujo una linea (imagen, punto inicio, punto fin)
	MyLine(img, Point(livesStart - 2, 2), Point(livesStart - 4, 22));	// "
}
void MyLine(Mat img, Point start, Point end) {
	Scalar color = Scalar(30, 30, 30);				// Seteo el color de linea (Blue=30, Green=30, Red=30), Gris oscuro
	int thickness = 1;								// Grosor de la linea
	int lineType = 8;								// Tipo de linea
	line(img,start,end,color,thickness,lineType);	// Dibujo la linea (imagen, punto inicio, punto fin, color, grosor, tipo de linea)
}
void MyText(Mat img, Point pt, char str[]) {
	int fontFace = FONT_HERSHEY_PLAIN;									// Defino la fuente
	double fontScale = 1;												// Defino el tamaño
	Scalar color = Scalar(30, 30, 30);									// Defino el color (Blue=30, Green=30, Red=30), Gris oscuro
	int thickness = 1;													// Grosor
	int linetype = 1;													// Tipo de linea
	putText(img,str,pt,fontFace,fontScale,color,thickness,linetype);	// Dibujo el texto (imagen, string a dibujar, punto de referencia, tamaño, color, grosor, tipo de linea)
}
void intToString(char *str, int n) {
	int index = digits(n) - 1;
	int dig = digits(n);
	while (n / 10 >= 1) {
		str[index] = (n % 10) + 48;
		n = n / 10;
		index--;
	}
	str[index] = n + 48;
	str[dig] = '\0';
	return;
}
int digits(int num) {
	int i = 1;
	while (num / 10 >= 1) {
		num = num / 10;
		i++;
	}
	return i;
}
int randomNum(int max, int min) {
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(min, max); // define the range
	return distr(eng);
}
Point setFruit() {
	return Point(randomNum(maxX, minX), randomNum(maxY, minY));
}
void printFruit(Mat img, Point c) {
	Scalar fruitColor = Scalar(0,0,255);									// Defino el color de la fruta (Blue=0, Green=0, Red=255), rojo
	Scalar woodColor = Scalar(0, 200, 255);									// Defino el color del palito (Blue=0, Green=200, Red=255), marron
	circle(img, Point(c.x-3,c.y), 4, fruitColor, -1, 8, 0);					// dibujo un circulo (imagen, punto central, radio, color, grosor, tipo de linea, shiftness(ni idea, siempre en 0 XD))
	circle(img, Point(c.x+3,c.y), 4, fruitColor, -1, 8, 0);					// "
	circle(img, Point(c.x + 5, c.y-2), 1, Scalar(255,255,255), -1, 8, 0);	// "
	line(img, Point(c.x,c.y-4), Point(c.x+2,c.y-8),woodColor,1,8);			// Dibujo el palito (imagen, punto inicial, punto final, color, grosor, tipo de linea)
}
/**
	Métodos de la clase
**/
Snake::Snake(int n) {
	len = n;
	int j = minX+(len*12);
	for (int i = 0; i < len; i++) {
		c[i].x = j;
		c[i].y = minY;
		j -= 12;
	}
	direction = RIGHT;
}
void Snake::move(int newDir) {
	int flag = 0;
	Point aux = c[0];
	Point aux2;
	switch (newDir) {
		case RIGHT: {
			if (direction != LEFT) {
				if (c[0].x >= maxX) {
					c[0].x = minX;
				}
				else {
					c[0].x = c[0].x + 12;
				}
				direction = RIGHT;
			}
			else {
				flag = -1;
			}
		} break;
		case LEFT: {
			if (direction != RIGHT) {
				if (c[0].x <= minX) {
					c[0].x = maxX;
				}
				else {
					c[0].x = c[0].x - 12;
				}
				direction = LEFT;
			}
			else {
				flag = -1;
			}
		} break;
		case UP: {
			if (direction != DOWN) {
				if (c[0].y <= minY) {
					c[0].y = maxY;
				}
				else {
					c[0].y = c[0].y - 12;
				}
				direction = UP;
			}
			else {
				flag = -1;
			}
		} break;
		case DOWN: {
			if (direction != UP) {
				if (c[0].y >= maxY) {
					c[0].y = minY;
				}
				else {
					c[0].y = c[0].y + 12;
				}
				direction = DOWN;
			}
			else {
				flag = -1;
			}
		} break;
		case ESC: {
			exit(0);
		} break;
		default: {
			return;
		}
	}
	if (flag == 0) {
		for (int i = 1; i<len; i++) {
			aux2 = c[i];
			c[i] = aux;
			aux = aux2;
		}
	}
}
bool Snake::contactS() {
	bool res = false;
	for (int i = 1; i < len; i++) {
		if ((c[i].x > c[0].x - 8 && c[i].x < c[0].x + 8) && (c[i].y > c[0].y - 8 && c[i].y < c[0].y + 8)) {
			res = true;
		}
	}
	return res;
}
bool Snake::contactF(Point p) {
	bool res=false;
	for (int i = 0; i < len; i++) {
		if ((c[i].x > p.x - 8 && c[i].x < p.x + 8) && (c[i].y > p.y - 8 && c[i].y < p.y + 8)) {
			res = true;
		}
	}
	return res;
}
void Snake::print(Mat img) {
	//Print Head
	circle(img, Point(c[0].x, c[0].y), 8, Scalar(0, 170, 0), -1, 8, 0); // Circulo (imagen, punto central, radio 8, color verde, relleno, tipo de linea, shiftness)
	circle(img, Point(c[0].x, c[0].y), 8, Scalar(30, 30, 30), 1, 8, 0);	// Circulo (imagen, punto central, radio 8, color negro, grosor 1, tipo de linea, shiftness)
	//Print Eyes
	switch (direction) {
		case LEFT: {
			circle(img, Point(c[0].x - 3, c[0].y - 3), 2, Scalar(250, 250, 250), -1, 8, 0);	// Circulo (imagen, centro, radio 2, color blanco, relleno, tipo de linea, shiftness)
			circle(img, Point(c[0].x - 3, c[0].y - 3), 2, Scalar(30,30,30), 1, 8, 0);		// Circulo (imagen, centro, radio 2, color negro, grosor 1, tipo de linea, shiftness)
			circle(img, Point(c[0].x - 3, c[0].y + 3), 2, Scalar(250, 250, 250), -1, 8, 0);	// Circulo (imagen, centro, radio 2, color blanco, relleno, tipo de linea, shiftness)
			circle(img, Point(c[0].x - 3, c[0].y + 3), 2, Scalar(30, 30, 30), 1, 8, 0);		// Circulo (imagen, centro, radio 2, color negro, grosor 1, tipo de linea, shiftness)
		} break;
		case RIGHT: {
			circle(img, Point(c[0].x + 3, c[0].y + 3), 2, Scalar(250, 250, 250), -1, 8, 0); // "
			circle(img, Point(c[0].x + 3, c[0].y + 3), 2, Scalar(30,30,30), 1, 8, 0);		// "
			circle(img, Point(c[0].x + 3, c[0].y - 3), 2, Scalar(250, 250, 250), -1, 8, 0);	// "
			circle(img, Point(c[0].x + 3, c[0].y - 3), 2, Scalar(30,30,30), 1, 8, 0);		// "
		} break;
		case UP: {
			circle(img, Point(c[0].x - 3, c[0].y - 3), 2, Scalar(250, 250, 250), -1, 8, 0); // "
			circle(img, Point(c[0].x - 3, c[0].y - 3), 2, Scalar(30,30,30), 1, 8, 0);		// "
			circle(img, Point(c[0].x + 3, c[0].y - 3), 2, Scalar(250, 250, 250), -1, 8, 0);	// "
			circle(img, Point(c[0].x + 3, c[0].y - 3), 2, Scalar(30,30,30), 1, 8, 0);		// "
		} break;
		case DOWN: {
			circle(img, Point(c[0].x - 3, c[0].y + 3), 2, Scalar(250, 250, 250), -1, 8, 0);	// "
			circle(img, Point(c[0].x - 3, c[0].y + 3), 2, Scalar(30,30,30), 1, 8, 0);		// "
			circle(img, Point(c[0].x + 3, c[0].y + 3), 2, Scalar(250, 250, 250), -1, 8, 0);	// "
			circle(img, Point(c[0].x + 3, c[0].y + 3), 2, Scalar(30,30,30), 1, 8, 0);		// "
		} break;
	}
	//Print body
	for (int i = 1; i < len-1; i++) {
		circle(img, Point(c[i].x, c[i].y), 6, Scalar(0, 170, 0), -1, 8, 0); 									// Circulo (imagen, centro, radio 6, color verde, relleno, tipo de linea, shiftness)
		circle(img, Point(c[i].x, c[i].y), 6, Scalar(30, 30, 30), 1, 8, 0);										// Circulo (imagen, centro, radio 6, color negro, grosor 1, tipo de linea, shiftness)
		line(img, Point(c[i].x - 3, c[i].y + 3), Point(c[i].x + 3, c[i].y - 3), Scalar(0, 100, 0), 1, 8, 0);	// Linea (imagen, inicio, fin, color verde oscuro, grosor 1, tipo de linea shiftness)
		line(img, Point(c[i].x - 4, c[i].y + 1), Point(c[i].x + 1, c[i].y - 4), Scalar(0, 100, 0), 1, 8, 0);	// Linea (imagen, inicio, fin, color verde oscuro, grosor 1, tipo de linea shiftness)
		line(img, Point(c[i].x - 1, c[i].y + 4), Point(c[i].x + 4, c[i].y - 1), Scalar(0, 100, 0), 1, 8, 0);	// Linea (imagen, inicio, fin, color verde oscuro, grosor 1, tipo de linea shiftness)
	}
	circle(img, Point(c[len-1].x, c[len - 1].y), 5, Scalar(0, 170, 0), -1, 8, 0);													// Circulo (imagen, centro, radio 5, color verde, tipo linea, shift)
	circle(img, Point(c[len - 1].x, c[len - 1].y), 5, Scalar(30, 30, 30), 1, 8, 0);													// Circulo (imagen, centro, radio 5, color verde, tipo linea, shift)
	line(img, Point(c[len - 1].x - 3, c[len - 1].y + 3), Point(c[len - 1].x + 3, c[len - 1].y - 3), Scalar(0, 100, 0), 1, 8, 0);	// Linea (imagen, inicio, fin, color verde oscuro, grosor 1, tipo linea, shift)
	line(img, Point(c[len - 1].x - 4, c[len - 1].y + 1), Point(c[len - 1].x + 1, c[len - 1].y - 4), Scalar(0, 100, 0), 1, 8, 0);	// Linea (imagen, inicio, fin, color verde oscuro, grosor 1, tipo linea, shift)
	line(img, Point(c[len - 1].x - 1, c[len - 1].y + 4), Point(c[len - 1].x + 4, c[len - 1].y - 1), Scalar(0, 100, 0), 1, 8, 0);	// Linea (imagen, inicio, fin, color verde oscuro, grosor 1, tipo linea, shift)
}
void Snake::enlarge() {
	if (len == 500) {
		return;
	}
	len++;
	c[len - 1] = c[len - 2];
	switch (direction) {
		case LEFT: {
			c[len - 1].x = c[len - 1].x + 12;
		} break;
		case RIGHT: {
			c[len - 1].x = c[len - 1].x - 12;
		} break;
		case DOWN: {
			c[len - 1].y = c[len - 1].y - 12;
		} break;
		case UP: {
			c[len - 1].y = c[len - 1].y + 12;
		} break;
	}
}
