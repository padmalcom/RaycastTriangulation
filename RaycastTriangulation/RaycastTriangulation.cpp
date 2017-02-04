// RaycastTriangulation.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//
#define NO_STDIO_REDIRECT
#include "stdafx.h"
#include <ctime>

#include "Vector2.h"
#include "Triangulator.h"
#include "TriangleIO.h"

//const double PI = atan(1.0) * 4;

int _tmain(int argc, _TCHAR* argv[])
{

	printf("Raycast Triangulator\n====================\nWritten by Jonas Freiknecht 2017\n\n");
	if (argc != 4) {
		printf("Expecting 3 parameters - input and output file and output type ('list' or 'array').\n");
		return 0;
	}

	bool asArray = false;
	if (argc == 4) {
		std::wstring wtype(argv[3]);
		std::string type(wtype.begin(), wtype.end());

		if (type == "array") {
			asArray = true;
		}
		else if (type == "list") {
			asArray = false;
		}
		else {
			printf("Could not recognize output type. Please specify 'list' or 'array' as 3rd parameter.\n");
		}
	}

	std::wstring win(argv[1]);
	std::wstring wout(argv[2]);
	std::string input(win.begin(), win.end());
	std::string output(wout.begin(), wout.end());

	printf("Reading input file %s...\n", input.c_str(), output.c_str());

	std::vector<Vector2> polygon;
	std::vector<std::vector<Vector2>*> holes;

	TriangleIO::readPolygon(input, polygon, holes);

	printf("Found polygon with %i points and %i holes.\n\n", polygon.size(), holes.size());

	unsigned int start = clock();
	
	// Germany
	/*polygon.push_back(Vector2(9.44535423355f, 54.8254001391f));
	polygon.push_back(Vector2(9.97200916046f, 54.7607542078f));
	polygon.push_back(Vector2(10.0325001824f, 54.555273192f));
	polygon.push_back(Vector2(9.86603608777f, 54.4572912581f));
	polygon.push_back(Vector2(10.9794451934f, 54.3805540962f));
	polygon.push_back(Vector2(11.0939542286f, 54.2054821222f));
	polygon.push_back(Vector2(10.7622181137f, 54.0565271528f));
	polygon.push_back(Vector2(10.8185361213f, 53.8900541734f));
	polygon.push_back(Vector2(11.0947181553f, 54.0136091263f));
	polygon.push_back(Vector2(11.4127731602f, 53.9197180641f));
	polygon.push_back(Vector2(12.5269451866f, 54.4741541386f));
	polygon.push_back(Vector2(12.9215271354f, 54.427464088f));
	polygon.push_back(Vector2(12.4638911464f, 54.3951360931f));
	polygon.push_back(Vector2(12.3743730907f, 54.2624272638f));
	polygon.push_back(Vector2(13.0238911282f, 54.3997181443f));
	polygon.push_back(Vector2(13.4545821483f, 54.0970821533f));
	polygon.push_back(Vector2(13.7108273227f, 54.1708269758f));
	polygon.push_back(Vector2(13.8085361741f, 53.8547821189f));
	polygon.push_back(Vector2(14.2756271176f, 53.6990641271f));
	polygon.push_back(Vector2(14.3916911635f, 53.1441641981f));
	polygon.push_back(Vector2(14.1491631414f, 52.8627820313f));
	polygon.push_back(Vector2(14.6395822626f, 52.5729822544f));
	polygon.push_back(Vector2(14.5344452009f, 52.3962451316f));
	polygon.push_back(Vector2(14.7607632928f, 52.0698642146f));
	polygon.push_back(Vector2(14.6009732048f, 51.8200640533f));
	polygon.push_back(Vector2(15.0338182064f, 51.2866539829f));
	polygon.push_back(Vector2(14.8283361848f, 50.8658271218f));
	polygon.push_back(Vector2(14.3062452041f, 51.0524911002f));
	polygon.push_back(Vector2(14.3113911898f, 50.8822181011f));
	polygon.push_back(Vector2(12.985554147f, 50.4183272007f));
	polygon.push_back(Vector2(12.5154180581f, 50.3924911575f));
	polygon.push_back(Vector2(12.3230542179f, 50.206664196f));
	polygon.push_back(Vector2(12.0937002006f, 50.3225361287f));
	polygon.push_back(Vector2(12.5459731127f, 49.9095822216f));
	polygon.push_back(Vector2(12.4555543378f, 49.6955451266f));
	polygon.push_back(Vector2(12.6744452253f, 49.4250002578f));
	polygon.push_back(Vector2(13.8336092964f, 48.7736090326f));
	polygon.push_back(Vector2(13.7260000762f, 48.5155910857f));
	polygon.push_back(Vector2(13.4432271841f, 48.5602361196f));
	polygon.push_back(Vector2(13.3950002276f, 48.3661001774f));
	polygon.push_back(Vector2(12.7597181822f, 48.1217271308f));
	polygon.push_back(Vector2(13.0088912095f, 47.8541642078f));
	polygon.push_back(Vector2(12.9139542535f, 47.7250000853f));
	polygon.push_back(Vector2(13.1001361048f, 47.642918119f));
	polygon.push_back(Vector2(13.0125001218f, 47.4697910819f));
	polygon.push_back(Vector2(12.7369452217f, 47.6827091928f));
	polygon.push_back(Vector2(11.104027097f, 47.3965270453f));
	polygon.push_back(Vector2(10.4818002011f, 47.5865181364f));
	polygon.push_back(Vector2(10.1733361051f, 47.2747182105f));
	polygon.push_back(Vector2(10.2317361774f, 47.3737450324f));
	polygon.push_back(Vector2(9.95500009958f, 47.5397181151f));
	polygon.push_back(Vector2(9.56672704172f, 47.5404542139f));
	polygon.push_back(Vector2(8.56291802597f, 47.80666413f));
	polygon.push_back(Vector2(8.40694522432f, 47.7018001508f));
	polygon.push_back(Vector2(8.5764180869f, 47.5913731023f));
	polygon.push_back(Vector2(7.58827317186f, 47.5844821721f));
	polygon.push_back(Vector2(7.57838219081f, 48.1172002325f));
	polygon.push_back(Vector2(7.80221806588f, 48.5758269625f));
	polygon.push_back(Vector2(8.22608219347f, 48.9644181974f));
	polygon.push_back(Vector2(7.42555411159f, 49.1764540292f));
	polygon.push_back(Vector2(6.72923622052f, 49.1676360994f));
	polygon.push_back(Vector2(6.36217306579f, 49.459391039f));
	polygon.push_back(Vector2(6.52402711365f, 49.8077001882f));
	polygon.push_back(Vector2(6.2341631314f, 49.8975000434f));
	polygon.push_back(Vector2(6.13440926324f, 50.1278451369f));
	polygon.push_back(Vector2(6.39820016212f, 50.3231729857f));
	polygon.push_back(Vector2(6.27041821549f, 50.6198541375f));
	polygon.push_back(Vector2(6.01180012435f, 50.7572730935f));
	polygon.push_back(Vector2(6.0808361613f, 50.9147180926f));
	polygon.push_back(Vector2(5.86500014228f, 51.0453451925f));
	polygon.push_back(Vector2(6.09750005541f, 51.1311001088f));
	polygon.push_back(Vector2(6.22208212668f, 51.4673542372f));
	polygon.push_back(Vector2(5.9636092101f, 51.8066642399f));
	polygon.push_back(Vector2(6.82895421414f, 51.9757540495f));
	polygon.push_back(Vector2(6.73639118073f, 52.0766641174f));
	polygon.push_back(Vector2(7.05309116718f, 52.2377641293f));
	polygon.push_back(Vector2(7.06298214823f, 52.3909641973f));
	polygon.push_back(Vector2(6.68958227468f, 52.5505541255f));
	polygon.push_back(Vector2(7.05347321433f, 52.6495821208f));
	polygon.push_back(Vector2(7.20836307409f, 53.2428091404f));
	polygon.push_back(Vector2(7.3370821175f, 53.3230539613f));
	polygon.push_back(Vector2(7.01500011101f, 53.409854101f));
	polygon.push_back(Vector2(7.2958362806f, 53.6852730465f));
	polygon.push_back(Vector2(8.0143092182f, 53.708327136f));
	polygon.push_back(Vector2(8.09777319277f, 53.4444452096f));
	polygon.push_back(Vector2(8.28603627067f, 53.4208910558f));
	polygon.push_back(Vector2(8.33221821036f, 53.6152732583f));
	polygon.push_back(Vector2(8.49206311602f, 53.5558822774f));
	polygon.push_back(Vector2(8.48410014035f, 53.6863181022f));
	polygon.push_back(Vector2(8.65902710735f, 53.8926361349f));
	polygon.push_back(Vector2(9.0960452909f, 53.8666731897f));
	polygon.push_back(Vector2(9.28340010577f, 53.8555452074f));
	polygon.push_back(Vector2(9.82906317748f, 53.5417000972f));
	polygon.push_back(Vector2(9.27600927881f, 53.8818912055f));
	polygon.push_back(Vector2(8.88305408982f, 53.9611090756f));
	polygon.push_back(Vector2(9.01721818518f, 54.0850003104f));
	polygon.push_back(Vector2(8.82736304858f, 54.1516642687f));
	polygon.push_back(Vector2(8.88360913945f, 54.2941641668f));
	polygon.push_back(Vector2(8.60048219571f, 54.3263821911f));
	polygon.push_back(Vector2(9.01124524096f, 54.5034639777f));
	polygon.push_back(Vector2(8.54560922823f, 54.8706091075f));
	polygon.push_back(Vector2(8.28055433834f, 54.7749912056f));
	polygon.push_back(Vector2(8.40805415011f, 55.0565270965f));
	polygon.push_back(Vector2(8.42041812788f, 54.9196450852f));
	polygon.push_back(Vector2(8.66454508186f, 54.9130911075f));*/
	

	// Rectangle
	/*polygon.push_back(Vector2(0, 0));
	polygon.push_back(Vector2(10, 0));
	polygon.push_back(Vector2(10, 10));
	polygon.push_back(Vector2(0, 10));*/

	// Circle
	/*for (int i = 0; i < 360; i += 1) {
		int depth = i % 2 == 0 ? 10000 : 20000;
		float x = depth * cos(i * PI / 180.0f);
		float y = depth * sin(i * PI / 180.0f);
		polygon.push_back(Vector2(x, y));
	}*/

	// Shell
	/*polygon.push_back(Vector2(10.0f, 0.0f));
	polygon.push_back(Vector2(20.0f, 0.0f));
	polygon.push_back(Vector2(30.0f, 10.0f));
	polygon.push_back(Vector2(30.0f, 20.0f));
	polygon.push_back(Vector2(20.0f, 30.0f));
	polygon.push_back(Vector2(10.0f, 30.0f));
	polygon.push_back(Vector2(0.0f, 20.0f));
	polygon.push_back(Vector2(0.0f, 10.0f));*/

	/*for (std::vector<Vector2>::size_type i = 0; i < polygon.size(); i++) {
		printf("Added vector (%f,%f).\n", polygon.at(i).x, polygon.at(i).y);
	}*/
	
	
	/*std::vector<Vector2> *hole1 = new std::vector<Vector2>();
	hole1->push_back(Vector2(2.5f, 2.5f));
	hole1->push_back(Vector2(7.5f, 2.5f));
	hole1->push_back(Vector2(7.5f, 7.5f));
	hole1->push_back(Vector2(2.5f, 7.5f));*/
	//holes.push_back(hole1);

	std::vector<Vector2> *vertices = NULL;
	std::vector<int> *indices = NULL;

	printf("Starting triangulation ...\n");
	Triangulator::triangulate(polygon, holes, indices, vertices);

	unsigned int stop = clock() - start;

	printf("Found %i vertices and %i indices in %i milliseconds.\n\n", vertices->size(), indices->size(), stop);

	TriangleIO::writeTriangle(output, *indices, *vertices, asArray);
	printf("Vertices and indices written to %s as %s. Done.\n", output.c_str(), (asArray ? "array" : "list"));

	return 0;
}

