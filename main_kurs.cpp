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

BOOL CALLBACK DlgProc(HWND,UINT,WPARAM,LPARAM); //������

//���� � ���������
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	int ret=DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
	return ret;
}

#define BufSize 32
char Buf[BufSize];

HWND hEdit1, hEdit2; //���������� ����� ��������������
HWND hStatic; //���������� ���� ������� ��������
HWND hButParam,hButStart,hButClose; //����������� ������

double Radius; //������ �����
double radius; //������ �������
double xM, yM, xMark, yMark, xThread, yThread; //���������� ������ ��������, ������� � ����� ����
double a; //��������� ��������
double fi; //���� �������� ��������
double length; //����� ���� ��������
string path = "result.txt";


double round(double chislo, int dx)
{
	return (round(chislo * dx)) / dx;
};


BOOL CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { //������� ��������� ��� ��������� ���������
	switch(msg) { //������������� ��������������� ��������� 	
		case WM_INITDIALOG: //��������� ����������� ����
			{
				//����������� ������������ ���� ��������������			
				hEdit1=GetDlgItem(hwnd,IDC_EDIT1); //hEdit1 - ������ �����
				hEdit2=GetDlgItem(hwnd,IDC_EDIT2); //hEdit2 - ������ �������
				hStatic=GetDlgItem(hwnd,IDC_STATIC6); //���������� ���� ������� ��������
				
				SetWindowText(hEdit1,"50"); //�������� �� ���������
				SetWindowText(hEdit2,"5");
				
				hButParam=GetDlgItem(hwnd,IDC_Param); //����������� ��������� ����
				hButStart=GetDlgItem(hwnd,IDC_St);
				hButClose=GetDlgItem(hwnd,IDC_Close);
				
				EnableWindow(hButStart,FALSE); //���������� ������
				
				return FALSE;			
			}
		case WM_COMMAND: //��������� ��������� �� ������ � ����� ����� ��������� ��������
			{
				switch (LOWORD(wParam)) {
					case IDC_EDIT1:
						GetWindowText(hEdit1,Buf,BufSize);
						Radius=atof(Buf); //�������������� ������ � double
					return TRUE;

					case IDC_EDIT2:
						GetWindowText(hEdit2,Buf,BufSize);	
						radius=atof(Buf);
					return TRUE;

					case IDC_Param: //����������� �������� ����������, �������� ��������
						GetWindowText(hEdit1,Buf,BufSize); //������������� ������ ���������� ������
						Radius=atof(Buf);
						GetWindowText(hEdit2,Buf,BufSize);	
						radius=atof(Buf);

						if (Radius < 5) { //������ �����������
							MessageBox(hwnd,
								"������ �����\n�� ����� ���� ����� �����!","",MB_OK);
							return TRUE;
						}
						if (radius < 4) {
							MessageBox(hwnd,
								"������ �������\n�� ����� ���� ����� �����!","",MB_OK);
							return TRUE;
						}
						if (radius > Radius) {
							MessageBox(hwnd,
								"������ ������� ������ ����\n������ ������� �����!","",MB_OK);
							return TRUE;
						}
						if (radius / Radius < 0.04) { 
							MessageBox(hwnd,
								"��������� ��������\n������� ������!", "", MB_OK);
							return TRUE;
						}
						if (Radius > 70) {
							MessageBox(hwnd,
								"������ �����\n������� �����!", "", MB_OK);
							return TRUE;
						}
						if (Radius - radius < 4) {
							MessageBox(hwnd,
								"��������� ��������\n������� ����!", "", MB_OK);
							return TRUE;
						}

						EnableWindow(hEdit1,FALSE); //������ ����� � ����� ��������������
						EnableWindow(hEdit2,FALSE);
						
						EnableWindow(hButParam,FALSE); //���������� ������
						EnableWindow(hButStart,TRUE); //��������� ������

						return TRUE;

					case IDC_St:	//�������������	�������� ��������			
						int dx, dy, Lx, Ly;
						dx = 20; dy = 40; Lx = 400, Ly = 530;
						
						HDC hDC;
						HRGN Maxwell, Disk, Marker, Thread;
						LOGBRUSH lb;
						HBRUSH hBrMaxwell, hBrDisk, hBrMark, hBrThread, hBrWh;
						hDC = GetDC(hwnd);

						Rectangle(hDC, dx, dy, dx + Lx, dy + Ly); //�����
						MoveToEx(hDC, dx + 170, dy + 10, NULL); //���������, � ������� �������� ����
						LineTo(hDC, dx + 230, dy + 10);

						lb.lbStyle = BS_SOLID;
						lb.lbColor = RGB(255, 255, 255);
						hBrWh = CreateBrushIndirect(&lb); //����� ������ �����
						
						xM = dx + 200 + radius; yM = dy + 20 + Radius;
						Maxwell = CreateEllipticRgn((int)(xM - radius), (int)(yM - radius),
							(int)(xM + radius), (int)(yM + radius)); //��������

						Disk = CreateEllipticRgn((int)(xM - Radius), (int)(yM - Radius),
							(int)(xM + Radius), (int)(yM + Radius)); //����
						
						lb.lbColor = RGB(105, 105, 105); //���� ����� - �����-�����
						hBrDisk = CreateBrushIndirect(&lb);
						FillRgn(hDC, Disk, hBrDisk);

						lb.lbColor = RGB(192, 192, 192); //���� ������� - ������-�����
						hBrMaxwell = CreateBrushIndirect(&lb);
						FillRgn(hDC, Maxwell, hBrMaxwell);
						
						xThread = dx + 200; yThread = dy + 20 + Radius; 
						Thread = CreateRectRgn((int)(dx + 199), (int)(dy + 10),
							(int)(dx + 200), (int)(yThread)); //����, ����������� ��������� � �������� ��������
						
						lb.lbColor = RGB(0, 0, 0); //���� ���� - ������
						hBrThread = CreateBrushIndirect(&lb);
						FillRgn(hDC, Thread, hBrThread);

						xMark = dx + 200 + radius; yMark = dy + 20 + Radius - 0.5 * (Radius - radius);
						Marker = CreateEllipticRgn((int)(xMark - 2), (int)(yMark - 2),
							(int)(xMark + 2), (int)(yMark + 2));//������, ������������ �������� ��������

						lb.lbColor = RGB(255, 0, 0);  //���� ������� - �������
						hBrMark = CreateBrushIndirect(&lb);
						FillRgn(hDC, Marker, hBrMark);

						a = 9.81 / (1 + (Radius / radius) * (Radius / radius)); //��������� �������� ���������
						fi = 0; //���� �������� �������

						length = 250; //����� ����

						//���������� ����� ���� ����� �������, ����� �� �������� ������� y_nizh � y_verh
						//�� ����� ���� � �� ��������� �������� ����� ����, � �� 2 � 4 case, � if ��������� �� ==, 
						//� >= (<=) length, ����� ����� ���� �� ������, ���������� f_nizh � f_verh, y_nizh � y_verh �
						//� ������� ���������� ����� ��������, ������� � ���� ��� ��������, �� �������� � fi
						double yThread_for_length = 0;
						int i = 1;
						do {
							yThread_for_length = 0.5 * a * i * i;
							i++;
						} while (yThread_for_length < length);
						length = yThread_for_length;

						int t_nizh = 0, t_verh = 0; //�������� ������� ����� � ������ ��������������
						double fi_nizh = 0, fi_verh = 0; //�������� ���� ����� � ������ ��������������
						double v = 0, w = 0; //�������� ��������������� � ������������� �������� �������� ���������
						double v0 = 0, w0 = 0; //�������� � ��������� ������ �������

						double rad_Mark = radius + 0.5 * (Radius - radius); //����������, �� ������� ������ ������� �� ������ ��������

						ofstream fout;
						fout.exceptions(ofstream::badbit | ofstream::failbit);
						double okrugl = 1000;

						SetWindowText(hStatic, "�������� �������� ���������");
						int for_case = 1;

						try {
							fout.open(path); // ��������� ����

							for (int i = 1; i <= 10 * 60; i++) { // �������� ������
								int t = GetTickCount();
								while ((GetTickCount() - t) < 100) {}; // i = 1 ������� * 1e-1 => 
								// => [at^2] = �� ; [vt] = �� ; [v] = �/� * 1�-1 ; [yM] = �� ;
								// => [(a/r)t^2] = [(epsilon)t^2] = ��� ; [wt] = ��� ; [w] = ���/� * 10 ; [fi] = ���

								switch (for_case) {
								case 1:
									yThread = (dy + 20 + Radius) + 0.5 * a * (i - t_verh) * (i - t_verh); //�-���������� ����� ����
									xM = (dx + 200 + radius); //x-���������� ������ ��������
									yM = (dy + 20 + Radius) + 0.5 * a * (i - t_verh) * (i - t_verh); //y-���������� ������ ��������

									fi = fi_verh + 0.5 * a * (1 / radius) * (i - t_verh) * (i - t_verh); //���� �������� �������
									xMark = xM + rad_Mark * sin(fi); //x-���������� �������
									yMark = yM - rad_Mark * cos(fi); //y-���������� �������

									v = a * (i - t_verh); //�������� ��������������� �������� ��������
									w = (a / radius) * (i - t_verh); //�������� ������������� ��������

									fout << '\t' << setw(8) << round(v, okrugl) << '\t' << setw(8) << round(w, okrugl);
									fout << '\t' << setw(8) << round(yM, okrugl) << '\t' << setw(8) << round(fi, okrugl) << endl;

									FillRgn(hDC, Disk, hBrWh); //������ ������� � �����
									DeleteObject(Disk); //�������
									FillRgn(hDC, Maxwell, hBrWh);
									DeleteObject(Maxwell);
									FillRgn(hDC, Thread, hBrWh);
									DeleteObject(Thread);

									Disk = CreateEllipticRgn((int)(xM - Radius), (int)(yM - Radius), //������� �������
										(int)(xM + Radius), (int)(yM + Radius));
									FillRgn(hDC, Disk, hBrDisk); //������
									Maxwell = CreateEllipticRgn((int)(xM - radius), (int)(yM - radius),
										(int)(xM + radius), (int)(yM + radius));
									FillRgn(hDC, Maxwell, hBrMaxwell);
									Marker = CreateEllipticRgn((int)(xMark - 2), (int)(yMark - 2),
										(int)(xMark + 2), (int)(yMark + 2));
									FillRgn(hDC, Marker, hBrMark);
									Thread = CreateRectRgn((int)(dx + 199), (int)(dy + 10),
										(int)(dx + 200), (int)(yM));
									FillRgn(hDC, Thread, hBrThread);

									if (yThread >= (dy + 20 + Radius) + length) { //����� �� ������
										t_nizh = i; //����� �����
										fi_nizh = fi; //���� � ������ ���������
										for_case = 2;
										v0 = v; //�������� �������� � ������ ���������
										w0 = w; //�������� �������� ������������� �������� � ������ ���������
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
						
						DeleteObject(Maxwell); //����������� �� �����
						DeleteObject(Disk);
						DeleteObject(Marker);
						DeleteObject(Thread);
						ReleaseDC(hwnd,hDC);

						EnableWindow(hEdit1,TRUE); //��������� ����� ��������������
						EnableWindow(hEdit2,TRUE);

						EnableWindow(hButParam,TRUE); //�� ����������
						EnableWindow(hButStart,FALSE); //��������� ������
					return TRUE;
				}
			}
		case IDC_Close: 
			{
				EndDialog(hwnd,0);
				return TRUE;
			}
		case WM_CLOSE: //�������� ����������� ����
		    { 
				EndDialog(hwnd,0);
				return TRUE; 
			}
	}
	return FALSE;
}