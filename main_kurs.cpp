#include <iostream>
#include <tchar.h>
#include <windows.h>
#include "resource_kurs.h"
#include <stdlib.h>
#include <math.h>

#include <string>
#include <fstream>
#include <cmath>
#include <iomanip>

using namespace std;

BOOL CALLBACK DlgProc(HWND,UINT,WPARAM,LPARAM); //шаблон

//вход в программу
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	int ret=DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
	return ret;
}

#define BufSize 32
char Buf[BufSize];

HWND hEdit1, hEdit2; //дескриптор полей редактирования
HWND hStatic; //дескриптор поля режимов движения
HWND hButParam,hButStart,hButClose; //дескрипторы кнопок

double Radius; //радиус диска
double radius; //радиус стержня
double xM, yM, xMark, yMark, xThread, yThread; //координаты центра маятника, маркера и конца нити
double a; //ускорение маятника
double fi; //угол поворота маятника
double length; //длина нити маятинка
string path = "result.txt";


double round(double chislo, int dx)
{
	return (round(chislo * dx)) / dx;
};


BOOL CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { //оконная процедура для обработки сообщений
	switch(msg) { //идентификатор обрабатываемого сообщения 	
		case WM_INITDIALOG: //инициация диалогового окна
			{
				//определение дескрипторов окон редактирования			
				hEdit1=GetDlgItem(hwnd,IDC_EDIT1); //hEdit1 - радиус диска
				hEdit2=GetDlgItem(hwnd,IDC_EDIT2); //hEdit2 - радиус стержня
				hStatic=GetDlgItem(hwnd,IDC_STATIC6); //дескриптор поля режимов движения
				
				SetWindowText(hEdit1,"50"); //значения по умолчанию
				SetWindowText(hEdit2,"5");
				
				hButParam=GetDlgItem(hwnd,IDC_Param); //дескрипторы кнопочных окон
				hButStart=GetDlgItem(hwnd,IDC_St);
				hButClose=GetDlgItem(hwnd,IDC_Close);
				
				EnableWindow(hButStart,FALSE); //отключение клавиш
				
				return FALSE;			
			}
		case WM_COMMAND: //обработка сообщений от кнопок и полей ввода численных значений
			{
				switch (LOWORD(wParam)) {
					case IDC_EDIT1:
						GetWindowText(hEdit1,Buf,BufSize);
						Radius=atof(Buf); //преобразование строки в double
					return TRUE;

					case IDC_EDIT2:
						GetWindowText(hEdit2,Buf,BufSize);	
						radius=atof(Buf);
					return TRUE;

					case IDC_Param: //определение значений параметров, проверка значений
						GetWindowText(hEdit1,Buf,BufSize); //подтверждение выбора параметров модели
						Radius=atof(Buf);
						GetWindowText(hEdit2,Buf,BufSize);	
						radius=atof(Buf);

						if (Radius < 5) { //всякие ограничения
							MessageBox(hwnd,
								"Радиус диска\nне может быть столь малым!","",MB_OK);
							return TRUE;
						}
						if (radius < 4) {
							MessageBox(hwnd,
								"Радиус стержня\nне может быть столь малым!","",MB_OK);
							return TRUE;
						}
						if (radius > Radius) {
							MessageBox(hwnd,
								"Радиус стержня должен быть\nменьше радиуса диска!","",MB_OK);
							return TRUE;
						}
						if (radius / Radius < 0.04) { 
							MessageBox(hwnd,
								"Отношение радиусов\nслишком велико!", "", MB_OK);
							return TRUE;
						}
						if (Radius > 70) {
							MessageBox(hwnd,
								"Радиус диска\nслишком велик!", "", MB_OK);
							return TRUE;
						}
						if (Radius - radius < 4) {
							MessageBox(hwnd,
								"Отношение радиусов\nслишком мало!", "", MB_OK);
							return TRUE;
						}

						EnableWindow(hEdit1,FALSE); //запрет ввода в полях редактирования
						EnableWindow(hEdit2,FALSE);
						
						EnableWindow(hButParam,FALSE); //блокировка кнопки
						EnableWindow(hButStart,TRUE); //активация кнопки

						return TRUE;

					case IDC_St:	//моделирование	движения маятника			
						int dx, dy, Lx, Ly;
						dx = 20; dy = 40; Lx = 400, Ly = 530;
						
						HDC hDC;
						HRGN Maxwell, Disk, Marker, Thread;
						LOGBRUSH lb;
						HBRUSH hBrMaxwell, hBrDisk, hBrMark, hBrThread, hBrWh;
						hDC = GetDC(hwnd);

						Rectangle(hDC, dx, dy, dx + Lx, dy + Ly); //рамка
						MoveToEx(hDC, dx + 170, dy + 10, NULL); //плоскость, к которой крепится нить
						LineTo(hDC, dx + 230, dy + 10);

						lb.lbStyle = BS_SOLID;
						lb.lbColor = RGB(255, 255, 255);
						hBrWh = CreateBrushIndirect(&lb); //кисть белого цвета
						
						xM = dx + 200 + radius; yM = dy + 20 + Radius;
						Maxwell = CreateEllipticRgn((int)(xM - radius), (int)(yM - radius),
							(int)(xM + radius), (int)(yM + radius)); //стержень

						Disk = CreateEllipticRgn((int)(xM - Radius), (int)(yM - Radius),
							(int)(xM + Radius), (int)(yM + Radius)); //диск
						
						lb.lbColor = RGB(105, 105, 105); //цвет диска - темно-серый
						hBrDisk = CreateBrushIndirect(&lb);
						FillRgn(hDC, Disk, hBrDisk);

						lb.lbColor = RGB(192, 192, 192); //цвет стержня - светло-серый
						hBrMaxwell = CreateBrushIndirect(&lb);
						FillRgn(hDC, Maxwell, hBrMaxwell);
						
						xThread = dx + 200; yThread = dy + 20 + Radius; 
						Thread = CreateRectRgn((int)(dx + 199), (int)(dy + 10),
							(int)(dx + 200), (int)(yThread)); //нить, соединяющая плоскость и стержень маятника
						
						lb.lbColor = RGB(0, 0, 0); //цвет нити - черный
						hBrThread = CreateBrushIndirect(&lb);
						FillRgn(hDC, Thread, hBrThread);

						xMark = dx + 200 + radius; yMark = dy + 20 + Radius - 0.5 * (Radius - radius);
						Marker = CreateEllipticRgn((int)(xMark - 2), (int)(yMark - 2),
							(int)(xMark + 2), (int)(yMark + 2));//маркер, изображающий вращение цилиндра

						lb.lbColor = RGB(255, 0, 0);  //цвет маркера - красный
						hBrMark = CreateBrushIndirect(&lb);
						FillRgn(hDC, Marker, hBrMark);

						a = 9.81 / (1 + (Radius / radius) * (Radius / radius)); //ускорение маятника Максвелла
						fi = 0; //угол поворота маркера

						length = 250; //длина нити

						//обновление длины нити таким образом, чтобы не пришлось вводить y_nizh и y_verh
						//но можно было и не подгонять значение длины нити, а во 2 и 4 case, в if прописать не ==, 
						//а >= (<=) length, тогда нужно было бы ввести, аналогично f_nizh и f_verh, y_nizh и y_verh и
						//и немного переписать закон движения, включив в него эти значения, по аналогии с fi
						double yThread_for_length = 0;
						int i = 1;
						do {
							yThread_for_length = 0.5 * a * i * i;
							i++;
						} while (yThread_for_length < length);
						length = yThread_for_length;

						int t_nizh = 0, t_verh = 0; //значение времени внизу и вверху соответственно
						double fi_nizh = 0, fi_verh = 0; //значение угла внизу и вверху соответственно
						double v = 0, w = 0; //скорость поступательного и вращательного движения маятника Максвелла
						double v0 = 0, w0 = 0; //скорости в некоторый момент времени

						double rad_Mark = radius + 0.5 * (Radius - radius); //расстояние, на которое маркер отстает от центра маятника

						ofstream fout;
						fout.exceptions(ofstream::badbit | ofstream::failbit);
						double okrugl = 1000;

						SetWindowText(hStatic, "Движение маятинка Максвелла");
						int for_case = 1;

						try {
							fout.open(path); // открываем файл

							for (int i = 1; i <= 10 * 60; i++) { // проходит минута
								int t = GetTickCount();
								while ((GetTickCount() - t) < 100) {}; // i = 1 секунда * 1e-1 => 
								// => [at^2] = см ; [vt] = см ; [v] = м/с * 1е-1 ; [yM] = см ;
								// => [(a/r)t^2] = [(epsilon)t^2] = рад ; [wt] = рад ; [w] = рад/с * 10 ; [fi] = рад

								switch (for_case) {
								case 1:
									yThread = (dy + 20 + Radius) + 0.5 * a * (i - t_verh) * (i - t_verh); //у-координата конца нити
									xM = (dx + 200 + radius); //x-координата центра маятника
									yM = (dy + 20 + Radius) + 0.5 * a * (i - t_verh) * (i - t_verh); //y-координата центра маятника

									fi = fi_verh + 0.5 * a * (1 / radius) * (i - t_verh) * (i - t_verh); //угол поворота маркера
									xMark = xM + rad_Mark * sin(fi); //x-координата маркера
									yMark = yM - rad_Mark * cos(fi); //y-координата маркера

									v = a * (i - t_verh); //скорость поступательного движения маятника
									w = (a / radius) * (i - t_verh); //скорость вращательного движения

									fout << '\t' << setw(8) << round(v, okrugl) << '\t' << setw(8) << round(w, okrugl);
									fout << '\t' << setw(8) << round(yM, okrugl) << '\t' << setw(8) << round(fi, okrugl) << endl;

									FillRgn(hDC, Disk, hBrWh); //красим объекты в белый
									DeleteObject(Disk); //удаляем
									FillRgn(hDC, Maxwell, hBrWh);
									DeleteObject(Maxwell);
									FillRgn(hDC, Thread, hBrWh);
									DeleteObject(Thread);

									Disk = CreateEllipticRgn((int)(xM - Radius), (int)(yM - Radius), //создаем объекты
										(int)(xM + Radius), (int)(yM + Radius));
									FillRgn(hDC, Disk, hBrDisk); //красим
									Maxwell = CreateEllipticRgn((int)(xM - radius), (int)(yM - radius),
										(int)(xM + radius), (int)(yM + radius));
									FillRgn(hDC, Maxwell, hBrMaxwell);
									Marker = CreateEllipticRgn((int)(xMark - 2), (int)(yMark - 2),
										(int)(xMark + 2), (int)(yMark + 2));
									FillRgn(hDC, Marker, hBrMark);
									Thread = CreateRectRgn((int)(dx + 199), (int)(dy + 10),
										(int)(dx + 200), (int)(yM));
									FillRgn(hDC, Thread, hBrThread);

									if (yThread >= (dy + 20 + Radius) + length) { //выход из кейсов
										t_nizh = i; //время внизу
										fi_nizh = fi; //угол в нижнем положении
										for_case = 2;
										v0 = v; //значение скорости в нижнем положении
										w0 = w; //значение скорости вращательного движения в нижнем положении
										fout << endl;
									}
									break;
								case 2:
									yThread = (dy + 20 + Radius + length) - 0.5 * a * (i - t_nizh) * (i - t_nizh);
									xM = (dx + 200 - radius);
									yM = (dy + 20 + Radius + length) - v0 * (i - t_nizh) + 0.5 * a * (i - t_nizh) * (i - t_nizh);

									fi = fi_nizh + w0 * (i - t_nizh) - 0.5 * a * (1 / radius) * (i - t_nizh) * (i - t_nizh);
									xMark = xM - rad_Mark * sin(fi);
									yMark = yM + rad_Mark * cos(fi);

									v = v0 - a * (i - t_nizh);
									w = w0 - (a / radius) * (i - t_nizh);

									fout << '\t' << setw(8) << round(v, okrugl) << '\t' << setw(8) << round(w, okrugl);
									fout << '\t' << setw(8) << round(yM, okrugl) << '\t' << setw(8) << round(fi, okrugl) << endl;

									FillRgn(hDC, Disk, hBrWh);
									DeleteObject(Disk);
									FillRgn(hDC, Maxwell, hBrWh);
									DeleteObject(Maxwell);
									FillRgn(hDC, Thread, hBrWh);
									DeleteObject(Thread);

									Disk = CreateEllipticRgn((int)(xM - Radius), (int)(yM - Radius),
										(int)(xM + Radius), (int)(yM + Radius));
									FillRgn(hDC, Disk, hBrDisk);
									Maxwell = CreateEllipticRgn((int)(xM - radius), (int)(yM - radius),
										(int)(xM + radius), (int)(yM + radius));
									FillRgn(hDC, Maxwell, hBrMaxwell);
									Marker = CreateEllipticRgn((int)(xMark - 2), (int)(yMark - 2),
										(int)(xMark + 2), (int)(yMark + 2));
									FillRgn(hDC, Marker, hBrMark);
									Thread = CreateRectRgn((int)(dx + 199), (int)(dy + 10),
										(int)(dx + 200), (int)(yM));
									FillRgn(hDC, Thread, hBrThread);

									if (yThread == dy + 20 + Radius) {
										t_verh = i;
										fi_verh = fi;
										for_case = 3;
										v0 = v;
										w0 = w;
										fout << endl;
									}
									break;
								case 3:
									yThread = (dy + 20 + Radius) + 0.5 * a * (i - t_verh) * (i - t_verh);
									xM = (dx + 200 - radius);
									yM = (dy + 20 + Radius) + 0.5 * a * (i - t_verh) * (i - t_verh);

									fi = fi_verh - 0.5 * a * (1 / radius) * (i - t_verh) * (i - t_verh);
									xMark = xM - rad_Mark * sin(fi);
									yMark = yM + rad_Mark * cos(fi);

									v = a * (i - t_verh);
									w = (a / radius) * (i - t_verh);

									fout << '\t' << setw(8) << round(v, okrugl) << '\t' << setw(8) << round(w, okrugl);
									fout << '\t' << setw(8) << round(yM, okrugl) << '\t' << setw(8) << round(fi, okrugl) << endl;

									FillRgn(hDC, Disk, hBrWh);
									DeleteObject(Disk);
									FillRgn(hDC, Maxwell, hBrWh);
									DeleteObject(Maxwell);
									FillRgn(hDC, Thread, hBrWh);
									DeleteObject(Thread);

									Disk = CreateEllipticRgn((int)(xM - Radius), (int)(yM - Radius),
										(int)(xM + Radius), (int)(yM + Radius));
									FillRgn(hDC, Disk, hBrDisk);
									Maxwell = CreateEllipticRgn((int)(xM - radius), (int)(yM - radius),
										(int)(xM + radius), (int)(yM + radius));
									FillRgn(hDC, Maxwell, hBrMaxwell);
									Marker = CreateEllipticRgn((int)(xMark - 2), (int)(yMark - 2),
										(int)(xMark + 2), (int)(yMark + 2));
									FillRgn(hDC, Marker, hBrMark);
									Thread = CreateRectRgn((int)(dx + 199), (int)(dy + 10),
										(int)(dx + 200), (int)(yM));
									FillRgn(hDC, Thread, hBrThread);

									if (yThread >= (dy + 20 + Radius) + length) {
										t_nizh = i;
										fi_nizh = fi;
										for_case = 4;
										v0 = v;
										w0 = w;
										fout << endl;
									}
									break;
								case 4:
									yThread = (dy + 20 + Radius + length) - 0.5 * a * (i - t_nizh) * (i - t_nizh);
									xM = (dx + 200 + radius);
									yM = (dy + 20 + Radius + length) - v0 * (i - t_nizh) + 0.5 * a * (i - t_nizh) * (i - t_nizh);

									fi = fi_nizh - w0 * (i - t_nizh) + 0.5 * a * (1 / radius) * (i - t_nizh) * (i - t_nizh);
									xMark = xM + rad_Mark * sin(fi);
									yMark = yM - rad_Mark * cos(fi);

									v = v0 - a * (i - t_nizh);
									w = w0 - (a / radius) * (i - t_nizh);

									fout << '\t' << setw(8) << round(v, okrugl) << '\t' << setw(8) << round(w, okrugl);
									fout << '\t' << setw(8) << round(yM, okrugl) << '\t' << setw(8) << round(fi, okrugl) << endl;

									FillRgn(hDC, Disk, hBrWh);
									DeleteObject(Disk);
									FillRgn(hDC, Maxwell, hBrWh);
									DeleteObject(Maxwell);
									FillRgn(hDC, Thread, hBrWh);
									DeleteObject(Thread);

									Disk = CreateEllipticRgn((int)(xM - Radius), (int)(yM - Radius),
										(int)(xM + Radius), (int)(yM + Radius));
									FillRgn(hDC, Disk, hBrDisk);
									Maxwell = CreateEllipticRgn((int)(xM - radius), (int)(yM - radius),
										(int)(xM + radius), (int)(yM + radius));
									FillRgn(hDC, Maxwell, hBrMaxwell);
									Marker = CreateEllipticRgn((int)(xMark - 2), (int)(yMark - 2),
										(int)(xMark + 2), (int)(yMark + 2));
									FillRgn(hDC, Marker, hBrMark);
									Thread = CreateRectRgn((int)(dx + 199), (int)(dy + 10),
										(int)(dx + 200), (int)(yM));
									FillRgn(hDC, Thread, hBrThread);

									if (yThread == dy + 20 + Radius) {
										t_verh = i;
										fi_verh = fi;
										for_case = 1;
										v0 = v;
										w0 = w;
										fout << endl;
									}
									break;
								}

							}

							fout.close();
						}
						catch (const ofstream::failure& exc) {}
						
						DeleteObject(Maxwell); //избавляемся от фигур
						DeleteObject(Disk);
						DeleteObject(Marker);
						DeleteObject(Thread);
						ReleaseDC(hwnd,hDC);

						EnableWindow(hEdit1,TRUE); //активация полей редактирования
						EnableWindow(hEdit2,TRUE);

						EnableWindow(hButParam,TRUE); //их блокировка
						EnableWindow(hButStart,FALSE); //активация старта
					return TRUE;
				}
			}
		case IDC_Close: 
			{
				EndDialog(hwnd,0);
				return TRUE;
			}
		case WM_CLOSE: //закрытие диалогового окна
		    { 
				EndDialog(hwnd,0);
				return TRUE; 
			}
	}
	return FALSE;
}