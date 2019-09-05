//#include "DHT.h" 
//Based on copyright LMTT20010101
 
//#define DHTpin 4  //Digital Input for Humid and temperature sensor.
//#define DHTtype DHT22 // DHT 22


// Initialize DHT sensor.
//DHT dht(DHTpin, DHTtype);


const int rules = 25;
const int inputs = 2; //NTI=NTV-1;

double yf = 0.0;

double X[2];

int DB[25][3] = {
{1, 1, 1},
{1, 2, 1},
{1, 3, 1},
{1, 4, 2},
{1, 5, 2},
{2, 1, 1},
{2, 2, 1},
{2, 3, 2},
{2, 4, 2},
{2, 5, 2},
{3, 1, 1},
{3, 2, 2},
{3, 3, 2},
{3, 4, 2},
{3, 5, 3},
{4, 1, 2},
{4, 2, 3},
{4, 3, 3},
{4, 4, 4},
{4, 5, 5},
{5, 1, 3},
{5, 2, 3},
{5, 3, 4},
{5, 4, 5},
{5, 5, 5},
};

double trapezoidmf(double x, double a, double b, double c, double d) {
double mf = max(min(min((x - a) / (b - a + 0.000001), 1.0), (d - x) / (d - c + 0.0000001)), 0.0);
return mf;
}

double trianglemf(double x, double a, double b, double c) {
double mf = max(min((x - a) / (b - a + 0.000001), (c - x) / (c - b + 0.000001)), 0.0);
return mf;
}
double Type1FS(double x, int n, double V[]) {
double a = V[0];
double b = V[1];
double c = V[2];
double mf;
if (n == 1) {
mf = trapezoidmf(x, a - 1.0001, a, b, c);
return mf;
}
if (n == 2) {
mf = trianglemf(x, a, b, c);
return mf;
} if (n == 3) {
mf = trianglemf(x, a, b, c);
return mf;
}
if (n == 4) {
mf = trianglemf(x, a, b, c);
return mf;
}
if (n == 5) {
mf = trapezoidmf(x, a, b, c, c + 1);
return mf;
}
if (n == 0) {
mf = 1;
return mf;
}
}

double FuzzySysT1(double X[], int DB[][3]) {
double PARAM[5][3] = {
{0 , 0.1666, 0.3333},
{ 0.1666, 0.3333, 0.5},
{ 0.3333, 0.5, 0.6666},
{0.5, 0.6666, 0.8333},
{0.6666, 0.8333, 1}
};
double V[3];
double AC[5] = {0, 0.25, 0.5, 0.75, 1};
double Fo[25];
for (int r = 0; r <= (rules - 1); r++) {
double sumin = 1;
int n;
for (int i = 0; i <= (inputs - 1); i++) {
n = DB[r][i] - 1;
if (n > -1) {
V[0] = PARAM[n][0];
V[1] = PARAM[n][1];
V[2] = PARAM[n][2];
}
double mf = Type1FS(X[i], (n + 1), V);
sumin = min(sumin, mf);
}
Fo[r] = sumin;
}
double sum1 = 0;
double sum2 = 0.00000001;
for (int r = 0; r <= (rules); r++)
{
sum1 = (sum1 + (Fo[r] * AC[DB[r][2] - 1]));
sum2 = (sum2 + Fo[r]);
}
double y = sum1 / sum2;
return y;
}

void setup()
{
  Serial.begin(115200);
 
}

 
void loop()
{
//   float t = dht.readTemperature();  
 
//  Serial.print("Temperature: ");
//  Serial.println(t);

Serial.println("Fuzzy System working ");
//read sensors
for (int i = 0; i < 2; i++) {
     
    X[i] = (double) (i+1)/2;
}

for (int i = 0; i < 2; i++) {
    Serial.print("X:");
    Serial.println(X[i]);
}


yf = FuzzySysT1(X, DB);
Serial.print("Output:");
Serial.println(yf);

Serial.println("Second input: ");

  X[0]=0.2; X[1]=0.3;

yf = FuzzySysT1(X, DB);
Serial.print("Output 2:");
Serial.println(yf);


delay(2500);
 
}
