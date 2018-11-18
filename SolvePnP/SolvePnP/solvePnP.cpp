#include"head.h"

void CodeRotateByZ(double x, double y, double thetaz, double& outx, double& outy)
{
	double x1 = x;
	double y1 = y;
	double rz = thetaz * CV_PI / 180;
	outx = cos(rz) * x1 - sin(rz) * y1;
	outy = sin(rz) * x1 + cos(rz) * y1;
}

//���ռ����Y����ת
//������� x zΪ�ռ��ԭʼx z����
//thetayΪ�ռ����Y����ת���ٶȣ��Ƕ��Ʒ�Χ��-180��180
//outx outzΪ��ת��Ľ������
void CodeRotateByY(double x, double z, double thetay, double& outx, double& outz)
{
	double x1 = x;
	double z1 = z;
	double ry = thetay * CV_PI / 180;
	outx = cos(ry) * x1 + sin(ry) * z1;
	outz = cos(ry) * z1 - sin(ry) * x1;
}

//���ռ����X����ת
//������� y zΪ�ռ��ԭʼy z����
//thetaxΪ�ռ����X����ת���ٶȣ��Ƕ��ƣ���Χ��-180��180
//outy outzΪ��ת��Ľ������
void CodeRotateByX(double y, double z, double thetax, double& outy, double& outz)
{
	double y1 = y;//����������һ�Σ���֤&y == &y���������Ҳ�ܼ�����ȷ
	double z1 = z;
	double rx = thetax * CV_PI / 180;
	outy = cos(rx) * y1 - sin(rx) * z1;
	outz = cos(rx) * z1 + sin(rx) * y1;
}

void solvepnp(vector<Point2f>&point_img) {
	vector<Point3f>point3D;
	//MATLAB�궨�õ�����ڲ��Լ��������
	double cameraMatrix[9] = {
						877.5879   ,  0 , 590.3458 ,
						0,  879.4849   ,403.7733,
						0, 0,   1.0000
	};
	Mat CameraMatrix = Mat(3, 3, CV_64FC1, cameraMatrix);
	double distCoeffD[5] = {0.0208 ,-0.0826,0,0,0};
	Mat distortion_coefficients = Mat(5, 1, CV_64FC1, distCoeffD);
	Mat rvec = Mat::zeros(3, 1, CV_64FC1);
	Mat tvec = Mat::zeros(3, 1, CV_64FC1);
	//������������ϵ
	point3D.push_back(Point3f(0,210, 0));
	point3D.push_back(Point3f(0, 0, 0));
	point3D.push_back(Point3f(300,0, 0));
	point3D.push_back(Point3f(300, 210, 0));
	//ʹ��solvepnp�õ���ת������ƽ������
	solvePnP(point3D, point_img, CameraMatrix, distortion_coefficients, rvec, tvec);
	//��ת��������ת����
	//��ȡ��ת����
	double rm[9];
	Mat rotM(3, 3, CV_64FC1, rm);
	Rodrigues(rvec, rotM);
	double r11 = rotM.ptr<double>(0)[0];
	double r12 = rotM.ptr<double>(0)[1];
	double r13 = rotM.ptr<double>(0)[2];
	double r21 = rotM.ptr<double>(1)[0];
	double r22 = rotM.ptr<double>(1)[1];
	double r23 = rotM.ptr<double>(1)[2];
	double r31 = rotM.ptr<double>(2)[0];
	double r32 = rotM.ptr<double>(2)[1];
	double r33 = rotM.ptr<double>(2)[2];

	//������������ϵ��������תŷ���ǣ���ת�����ת����������ϵ��
	double thetaz = atan2(r21, r11) / CV_PI * 180;
	double thetay = atan2(-1 * r31, sqrt(r32*r32 + r33 * r33)) / CV_PI * 180;
	double thetax = atan2(r32, r33) / CV_PI * 180;
	cout << "�����������ת�ǣ�" << -1 * thetax << ", " << -1 * thetay << ", " << -1 * thetaz << endl;
	

	//���ƽ�ƾ��󣬱�ʾ���������ϵԭ�㣬��������(x,y,z)�ߣ��͵�����������ϵԭ��
	double tx = tvec.ptr<double>(0)[0];
	double ty = tvec.ptr<double>(0)[1];
	double tz = tvec.ptr<double>(0)[2];

	//x y z ΪΨһ���������ԭʼ����ϵ�µ�����ֵ
	//Ҳ��������OcOw���������ϵ�µ�ֵ
	double x = tx, y = ty, z = tz;

	//�������η�����ת
	CodeRotateByZ(x, y, -1 * thetaz, x, y);
	CodeRotateByY(x, z, -1 * thetay, x, z);
	CodeRotateByX(y, z, -1 * thetax, y, z);


	//����������������ϵ�µ�λ������
	//������OcOw����������ϵ�µ�ֵ
	double Cx = x * -1;
	double Cy = y * -1;
	double Cz = z * -1;
	cout << "������������꣺" << Cx << ", " << Cy << ", " << Cz << endl;

}