///////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2021.                 //
//  Distributed under the Boost Software License,                //
//  Version 1.0. (See accompanying file LICENSE_1_0.txt          //
//  or copy at http://www.boost.org/LICENSE_1_0.txt)             //
///////////////////////////////////////////////////////////////////

#include <cstdint>

#include <math/wide_decimal/decwide_t.h>

bool math::wide_decimal::example001d_pow2_from_list()
{
  using local_limb_type = std::uint32_t;

  constexpr std::uint32_t wide_decimal_digits10 = UINT32_C(100);

  using wide_decimal_type = math::wide_decimal::decwide_t<wide_decimal_digits10, local_limb_type, void>;

  static const std::array<wide_decimal_type, 256U> local_pow2_data =
  {{
    wide_decimal_type( {       29UL, 38735877UL,  5571876UL, 99218413UL, 43055614UL, 19454666UL, 38919302UL, 18803771UL, 87926569UL, 60431486UL, 36817932UL, 12890625UL }, -40 ),
    wide_decimal_type( {       58UL, 77471754UL, 11143753UL, 98436826UL, 86111228UL, 38909332UL, 77838604UL, 37607543UL, 75853139UL, 20862972UL, 73635864UL, 25781250UL }, -40 ),
    wide_decimal_type( {      117UL, 54943508UL, 22287507UL, 96873653UL, 72222456UL, 77818665UL, 55677208UL, 75215087UL, 51706278UL, 41725945UL, 47271728UL, 51562500UL }, -40 ),
    wide_decimal_type( {      235UL,  9887016UL, 44575015UL, 93747307UL, 44444913UL, 55637331UL, 11354417UL, 50430175UL,  3412556UL, 83451890UL, 94543457UL,  3125000UL }, -40 ),
    wide_decimal_type( {      470UL, 19774032UL, 89150031UL, 87494614UL, 88889827UL, 11274662UL, 22708835UL,   860350UL,  6825113UL, 66903781UL, 89086914UL,  6250000UL }, -40 ),
    wide_decimal_type( {      940UL, 39548065UL, 78300063UL, 74989229UL, 77779654UL, 22549324UL, 45417670UL,  1720700UL, 13650227UL, 33807563UL, 78173828UL, 12500000UL }, -40 ),
    wide_decimal_type( {     1880UL, 79096131UL, 56600127UL, 49978459UL, 55559308UL, 45098648UL, 90835340UL,  3441400UL, 27300454UL, 67615127UL, 56347656UL, 25000000UL }, -40 ),
    wide_decimal_type( {     3761UL, 58192263UL, 13200254UL, 99956919UL, 11118616UL, 90197297UL, 81670680UL,  6882800UL, 54600909UL, 35230255UL, 12695312UL, 50000000UL }, -40 ),
    wide_decimal_type( {     7523UL, 16384526UL, 26400509UL, 99913838UL, 22237233UL, 80394595UL, 63341360UL, 13765601UL,  9201818UL, 70460510UL, 25390625UL }, -40 ),
    wide_decimal_type( {    15046UL, 32769052UL, 52801019UL, 99827676UL, 44474467UL, 60789191UL, 26682720UL, 27531202UL, 18403637UL, 40921020UL, 50781250UL }, -40 ),
    wide_decimal_type( {    30092UL, 65538105UL,  5602039UL, 99655352UL, 88948935UL, 21578382UL, 53365440UL, 55062404UL, 36807274UL, 81842041UL,  1562500UL }, -40 ),
    wide_decimal_type( {    60185UL, 31076210UL, 11204079UL, 99310705UL, 77897870UL, 43156765UL,  6730881UL, 10124808UL, 73614549UL, 63684082UL,  3125000UL }, -40 ),
    wide_decimal_type( {   120370UL, 62152420UL, 22408159UL, 98621411UL, 55795740UL, 86313530UL, 13461762UL, 20249617UL, 47229099UL, 27368164UL,  6250000UL }, -40 ),
    wide_decimal_type( {   240741UL, 24304840UL, 44816319UL, 97242823UL, 11591481UL, 72627060UL, 26923524UL, 40499234UL, 94458198UL, 54736328UL, 12500000UL }, -40 ),
    wide_decimal_type( {   481482UL, 48609680UL, 89632639UL, 94485646UL, 23182963UL, 45254120UL, 53847048UL, 80998469UL, 88916397UL,  9472656UL, 25000000UL }, -40 ),
    wide_decimal_type( {   962964UL, 97219361UL, 79265279UL, 88971292UL, 46365926UL, 90508241UL,  7694097UL, 61996939UL, 77832794UL, 18945312UL, 50000000UL }, -40 ),
    wide_decimal_type( {  1925929UL, 94438723UL, 58530559UL, 77942584UL, 92731853UL, 81016482UL, 15388195UL, 23993879UL, 55665588UL, 37890625UL }, -40 ),
    wide_decimal_type( {  3851859UL, 88877447UL, 17061119UL, 55885169UL, 85463707UL, 62032964UL, 30776390UL, 47987759UL, 11331176UL, 75781250UL }, -40 ),
    wide_decimal_type( {  7703719UL, 77754894UL, 34122239UL, 11770339UL, 70927415UL, 24065928UL, 61552780UL, 95975518UL, 22662353UL, 51562500UL }, -40 ),
    wide_decimal_type( { 15407439UL, 55509788UL, 68244478UL, 23540679UL, 41854830UL, 48131857UL, 23105561UL, 91951036UL, 45324707UL,  3125000UL }, -40 ),
    wide_decimal_type( { 30814879UL, 11019577UL, 36488956UL, 47081358UL, 83709660UL, 96263714UL, 46211123UL, 83902072UL, 90649414UL,  6250000UL }, -40 ),
    wide_decimal_type( { 61629758UL, 22039154UL, 72977912UL, 94162717UL, 67419321UL, 92527428UL, 92422247UL, 67804145UL, 81298828UL, 12500000UL }, -40 ),
    wide_decimal_type( {        1UL, 23259516UL, 44078309UL, 45955825UL, 88325435UL, 34838643UL, 85054857UL, 84844495UL, 35608291UL, 62597656UL, 25000000UL }, -32 ),
    wide_decimal_type( {        2UL, 46519032UL, 88156618UL, 91911651UL, 76650870UL, 69677287UL, 70109715UL, 69688990UL, 71216583UL, 25195312UL, 50000000UL }, -32 ),
    wide_decimal_type( {        4UL, 93038065UL, 76313237UL, 83823303UL, 53301741UL, 39354575UL, 40219431UL, 39377981UL, 42433166UL, 50390625UL }, -32 ),
    wide_decimal_type( {        9UL, 86076131UL, 52626475UL, 67646607UL,  6603482UL, 78709150UL, 80438862UL, 78755962UL, 84866333UL,   781250UL }, -32 ),
    wide_decimal_type( {       19UL, 72152263UL,  5252951UL, 35293214UL, 13206965UL, 57418301UL, 60877725UL, 57511925UL, 69732666UL,  1562500UL }, -32 ),
    wide_decimal_type( {       39UL, 44304526UL, 10505902UL, 70586428UL, 26413931UL, 14836603UL, 21755451UL, 15023851UL, 39465332UL,  3125000UL }, -32 ),
    wide_decimal_type( {       78UL, 88609052UL, 21011805UL, 41172856UL, 52827862UL, 29673206UL, 43510902UL, 30047702UL, 78930664UL,  6250000UL }, -32 ),
    wide_decimal_type( {      157UL, 77218104UL, 42023610UL, 82345713UL,  5655724UL, 59346412UL, 87021804UL, 60095405UL, 57861328UL, 12500000UL }, -32 ),
    wide_decimal_type( {      315UL, 54436208UL, 84047221UL, 64691426UL, 11311449UL, 18692825UL, 74043609UL, 20190811UL, 15722656UL, 25000000UL }, -32 ),
    wide_decimal_type( {      631UL,  8872417UL, 68094443UL, 29382852UL, 22622898UL, 37385651UL, 48087218UL, 40381622UL, 31445312UL, 50000000UL }, -32 ),
    wide_decimal_type( {     1262UL, 17744835UL, 36188886UL, 58765704UL, 45245796UL, 74771302UL, 96174436UL, 80763244UL, 62890625UL }, -32 ),
    wide_decimal_type( {     2524UL, 35489670UL, 72377773UL, 17531408UL, 90491593UL, 49542605UL, 92348873UL, 61526489UL, 25781250UL }, -32 ),
    wide_decimal_type( {     5048UL, 70979341UL, 44755546UL, 35062817UL, 80983186UL, 99085211UL, 84697747UL, 23052978UL, 51562500UL }, -32 ),
    wide_decimal_type( {    10097UL, 41958682UL, 89511092UL, 70125635UL, 61966373UL, 98170423UL, 69395494UL, 46105957UL,  3125000UL }, -32 ),
    wide_decimal_type( {    20194UL, 83917365UL, 79022185UL, 40251271UL, 23932747UL, 96340847UL, 38790988UL, 92211914UL,  6250000UL }, -32 ),
    wide_decimal_type( {    40389UL, 67834731UL, 58044370UL, 80502542UL, 47865495UL, 92681694UL, 77581977UL, 84423828UL, 12500000UL }, -32 ),
    wide_decimal_type( {    80779UL, 35669463UL, 16088741UL, 61005084UL, 95730991UL, 85363389UL, 55163955UL, 68847656UL, 25000000UL }, -32 ),
    wide_decimal_type( {   161558UL, 71338926UL, 32177483UL, 22010169UL, 91461983UL, 70726779UL, 10327911UL, 37695312UL, 50000000UL }, -32 ),
    wide_decimal_type( {   323117UL, 42677852UL, 64354966UL, 44020339UL, 82923967UL, 41453558UL, 20655822UL, 75390625UL }, -32 ),
    wide_decimal_type( {   646234UL, 85355705UL, 28709932UL, 88040679UL, 65847934UL, 82907116UL, 41311645UL, 50781250UL }, -32 ),
    wide_decimal_type( {  1292469UL, 70711410UL, 57419865UL, 76081359UL, 31695869UL, 65814232UL, 82623291UL,  1562500UL }, -32 ),
    wide_decimal_type( {  2584939UL, 41422821UL, 14839731UL, 52162718UL, 63391739UL, 31628465UL, 65246582UL,  3125000UL }, -32 ),
    wide_decimal_type( {  5169878UL, 82845642UL, 29679463UL,  4325437UL, 26783478UL, 63256931UL, 30493164UL,  6250000UL }, -32 ),
    wide_decimal_type( { 10339757UL, 65691284UL, 59358926UL,  8650874UL, 53566957UL, 26513862UL, 60986328UL, 12500000UL }, -32 ),
    wide_decimal_type( { 20679515UL, 31382569UL, 18717852UL, 17301749UL,  7133914UL, 53027725UL, 21972656UL, 25000000UL }, -32 ),
    wide_decimal_type( { 41359030UL, 62765138UL, 37435704UL, 34603498UL, 14267829UL,  6055450UL, 43945312UL, 50000000UL }, -32 ),
    wide_decimal_type( { 82718061UL, 25530276UL, 74871408UL, 69206996UL, 28535658UL, 12110900UL, 87890625UL }, -32 ),
    wide_decimal_type( {        1UL, 65436122UL, 51060553UL, 49742817UL, 38413992UL, 57071316UL, 24221801UL, 75781250UL }, -24 ),
    wide_decimal_type( {        3UL, 30872245UL,  2121106UL, 99485634UL, 76827985UL, 14142632UL, 48443603UL, 51562500UL }, -24 ),
    wide_decimal_type( {        6UL, 61744490UL,  4242213UL, 98971269UL, 53655970UL, 28285264UL, 96887207UL,  3125000UL }, -24 ),
    wide_decimal_type( {       13UL, 23488980UL,  8484427UL, 97942539UL,  7311940UL, 56570529UL, 93774414UL,  6250000UL }, -24 ),
    wide_decimal_type( {       26UL, 46977960UL, 16968855UL, 95885078UL, 14623881UL, 13141059UL, 87548828UL, 12500000UL }, -24 ),
    wide_decimal_type( {       52UL, 93955920UL, 33937711UL, 91770156UL, 29247762UL, 26282119UL, 75097656UL, 25000000UL }, -24 ),
    wide_decimal_type( {      105UL, 87911840UL, 67875423UL, 83540312UL, 58495524UL, 52564239UL, 50195312UL, 50000000UL }, -24 ),
    wide_decimal_type( {      211UL, 75823681UL, 35750847UL, 67080625UL, 16991049UL,  5128479UL,   390625UL }, -24 ),
    wide_decimal_type( {      423UL, 51647362UL, 71501695UL, 34161250UL, 33982098UL, 10256958UL,   781250UL }, -24 ),
    wide_decimal_type( {      847UL,  3294725UL, 43003390UL, 68322500UL, 67964196UL, 20513916UL,  1562500UL }, -24 ),
    wide_decimal_type( {     1694UL,  6589450UL, 86006781UL, 36645001UL, 35928392UL, 41027832UL,  3125000UL }, -24 ),
    wide_decimal_type( {     3388UL, 13178901UL, 72013562UL, 73290002UL, 71856784UL, 82055664UL,  6250000UL }, -24 ),
    wide_decimal_type( {     6776UL, 26357803UL, 44027125UL, 46580005UL, 43713569UL, 64111328UL, 12500000UL }, -24 ),
    wide_decimal_type( {    13552UL, 52715606UL, 88054250UL, 93160010UL, 87427139UL, 28222656UL, 25000000UL }, -24 ),
    wide_decimal_type( {    27105UL,  5431213UL, 76108501UL, 86320021UL, 74854278UL, 56445312UL, 50000000UL }, -24 ),
    wide_decimal_type( {    54210UL, 10862427UL, 52217003UL, 72640043UL, 49708557UL, 12890625UL }, -24 ),
    wide_decimal_type( {   108420UL, 21724855UL,  4434007UL, 45280086UL, 99417114UL, 25781250UL }, -24 ),
    wide_decimal_type( {   216840UL, 43449710UL,  8868014UL, 90560173UL, 98834228UL, 51562500UL }, -24 ),
    wide_decimal_type( {   433680UL, 86899420UL, 17736029UL, 81120347UL, 97668457UL,  3125000UL }, -24 ),
    wide_decimal_type( {   867361UL, 73798840UL, 35472059UL, 62240695UL, 95336914UL,  6250000UL }, -24 ),
    wide_decimal_type( {  1734723UL, 47597680UL, 70944119UL, 24481391UL, 90673828UL, 12500000UL }, -24 ),
    wide_decimal_type( {  3469446UL, 95195361UL, 41888238UL, 48962783UL, 81347656UL, 25000000UL }, -24 ),
    wide_decimal_type( {  6938893UL, 90390722UL, 83776476UL, 97925567UL, 62695312UL, 50000000UL }, -24 ),
    wide_decimal_type( { 13877787UL, 80781445UL, 67552953UL, 95851135UL, 25390625UL }, -24 ),
    wide_decimal_type( { 27755575UL, 61562891UL, 35105907UL, 91702270UL, 50781250UL }, -24 ),
    wide_decimal_type( { 55511151UL, 23125782UL, 70211815UL, 83404541UL,  1562500UL }, -24 ),
    wide_decimal_type( {        1UL, 11022302UL, 46251565UL, 40423631UL, 66809082UL,  3125000UL }, -16 ),
    wide_decimal_type( {        2UL, 22044604UL, 92503130UL, 80847263UL, 33618164UL,  6250000UL }, -16 ),
    wide_decimal_type( {        4UL, 44089209UL, 85006261UL, 61694526UL, 67236328UL, 12500000UL }, -16 ),
    wide_decimal_type( {        8UL, 88178419UL, 70012523UL, 23389053UL, 34472656UL, 25000000UL }, -16 ),
    wide_decimal_type( {       17UL, 76356839UL, 40025046UL, 46778106UL, 68945312UL, 50000000UL }, -16 ),
    wide_decimal_type( {       35UL, 52713678UL, 80050092UL, 93556213UL, 37890625UL }, -16 ),
    wide_decimal_type( {       71UL,  5427357UL, 60100185UL, 87112426UL, 75781250UL }, -16 ),
    wide_decimal_type( {      142UL, 10854715UL, 20200371UL, 74224853UL, 51562500UL }, -16 ),
    wide_decimal_type( {      284UL, 21709430UL, 40400743UL, 48449707UL,  3125000UL }, -16 ),
    wide_decimal_type( {      568UL, 43418860UL, 80801486UL, 96899414UL,  6250000UL }, -16 ),
    wide_decimal_type( {     1136UL, 86837721UL, 61602973UL, 93798828UL, 12500000UL }, -16 ),
    wide_decimal_type( {     2273UL, 73675443UL, 23205947UL, 87597656UL, 25000000UL }, -16 ),
    wide_decimal_type( {     4547UL, 47350886UL, 46411895UL, 75195312UL, 50000000UL }, -16 ),
    wide_decimal_type( {     9094UL, 94701772UL, 92823791UL, 50390625UL }, -16 ),
    wide_decimal_type( {    18189UL, 89403545UL, 85647583UL,   781250UL }, -16 ),
    wide_decimal_type( {    36379UL, 78807091UL, 71295166UL,  1562500UL }, -16 ),
    wide_decimal_type( {    72759UL, 57614183UL, 42590332UL,  3125000UL }, -16 ),
    wide_decimal_type( {   145519UL, 15228366UL, 85180664UL,  6250000UL }, -16 ),
    wide_decimal_type( {   291038UL, 30456733UL, 70361328UL, 12500000UL }, -16 ),
    wide_decimal_type( {   582076UL, 60913467UL, 40722656UL, 25000000UL }, -16 ),
    wide_decimal_type( {  1164153UL, 21826934UL, 81445312UL, 50000000UL }, -16 ),
    wide_decimal_type( {  2328306UL, 43653869UL, 62890625UL }, -16 ),
    wide_decimal_type( {  4656612UL, 87307739UL, 25781250UL }, -16 ),
    wide_decimal_type( {  9313225UL, 74615478UL, 51562500UL }, -16 ),
    wide_decimal_type( { 18626451UL, 49230957UL,  3125000UL }, -16 ),
    wide_decimal_type( { 37252902UL, 98461914UL,  6250000UL }, -16 ),
    wide_decimal_type( { 74505805UL, 96923828UL, 12500000UL }, -16 ),
    wide_decimal_type( {        1UL, 49011611UL, 93847656UL, 25000000UL }, -8 ),
    wide_decimal_type( {        2UL, 98023223UL, 87695312UL, 50000000UL }, -8 ),
    wide_decimal_type( {        5UL, 96046447UL, 75390625UL }, -8 ),
    wide_decimal_type( {       11UL, 92092895UL, 50781250UL }, -8 ),
    wide_decimal_type( {       23UL, 84185791UL,  1562500UL }, -8 ),
    wide_decimal_type( {       47UL, 68371582UL,  3125000UL }, -8 ),
    wide_decimal_type( {       95UL, 36743164UL,  6250000UL }, -8 ),
    wide_decimal_type( {      190UL, 73486328UL, 12500000UL }, -8 ),
    wide_decimal_type( {      381UL, 46972656UL, 25000000UL }, -8 ),
    wide_decimal_type( {      762UL, 93945312UL, 50000000UL }, -8 ),
    wide_decimal_type( {     1525UL, 87890625UL }, -8 ),
    wide_decimal_type( {     3051UL, 75781250UL }, -8 ),
    wide_decimal_type( {     6103UL, 51562500UL }, -8 ),
    wide_decimal_type( {    12207UL,  3125000UL }, -8 ),
    wide_decimal_type( {    24414UL,  6250000UL }, -8 ),
    wide_decimal_type( {    48828UL, 12500000UL }, -8 ),
    wide_decimal_type( {    97656UL, 25000000UL }, -8 ),
    wide_decimal_type( {   195312UL, 50000000UL }, -8 ),
    wide_decimal_type( {   390625UL }, -8 ),
    wide_decimal_type( {   781250UL }, -8 ),
    wide_decimal_type( {  1562500UL }, -8 ),
    wide_decimal_type( {  3125000UL }, -8 ),
    wide_decimal_type( {  6250000UL }, -8 ),
    wide_decimal_type( { 12500000UL }, -8 ),
    wide_decimal_type( { 25000000UL }, -8 ),
    wide_decimal_type( { 50000000UL }, -8 ),
    wide_decimal_type( {        1UL },  0 ),
    wide_decimal_type( {        2UL },  0 ),
    wide_decimal_type( {        4UL },  0 ),
    wide_decimal_type( {        8UL },  0 ),
    wide_decimal_type( {       16UL },  0 ),
    wide_decimal_type( {       32UL },  0 ),
    wide_decimal_type( {       64UL },  0 ),
    wide_decimal_type( {      128UL },  0 ),
    wide_decimal_type( {      256UL },  0 ),
    wide_decimal_type( {      512UL },  0 ),
    wide_decimal_type( {     1024UL },  0 ),
    wide_decimal_type( {     2048UL },  0 ),
    wide_decimal_type( {     4096UL },  0 ),
    wide_decimal_type( {     8192UL },  0 ),
    wide_decimal_type( {    16384UL },  0 ),
    wide_decimal_type( {    32768UL },  0 ),
    wide_decimal_type( {    65536UL },  0 ),
    wide_decimal_type( {   131072UL },  0 ),
    wide_decimal_type( {   262144UL },  0 ),
    wide_decimal_type( {   524288UL },  0 ),
    wide_decimal_type( {  1048576UL },  0 ),
    wide_decimal_type( {  2097152UL },  0 ),
    wide_decimal_type( {  4194304UL },  0 ),
    wide_decimal_type( {  8388608UL },  0 ),
    wide_decimal_type( { 16777216UL },  0 ),
    wide_decimal_type( { 33554432UL },  0 ),
    wide_decimal_type( { 67108864UL },  0 ),
    wide_decimal_type( {        1UL, 34217728UL },  8 ),
    wide_decimal_type( {        2UL, 68435456UL },  8 ),
    wide_decimal_type( {        5UL, 36870912UL },  8 ),
    wide_decimal_type( {       10UL, 73741824UL },  8 ),
    wide_decimal_type( {       21UL, 47483648UL },  8 ),
    wide_decimal_type( {       42UL, 94967296UL },  8 ),
    wide_decimal_type( {       85UL, 89934592UL },  8 ),
    wide_decimal_type( {      171UL, 79869184UL },  8 ),
    wide_decimal_type( {      343UL, 59738368UL },  8 ),
    wide_decimal_type( {      687UL, 19476736UL },  8 ),
    wide_decimal_type( {     1374UL, 38953472UL },  8 ),
    wide_decimal_type( {     2748UL, 77906944UL },  8 ),
    wide_decimal_type( {     5497UL, 55813888UL },  8 ),
    wide_decimal_type( {    10995UL, 11627776UL },  8 ),
    wide_decimal_type( {    21990UL, 23255552UL },  8 ),
    wide_decimal_type( {    43980UL, 46511104UL },  8 ),
    wide_decimal_type( {    87960UL, 93022208UL },  8 ),
    wide_decimal_type( {   175921UL, 86044416UL },  8 ),
    wide_decimal_type( {   351843UL, 72088832UL },  8 ),
    wide_decimal_type( {   703687UL, 44177664UL },  8 ),
    wide_decimal_type( {  1407374UL, 88355328UL },  8 ),
    wide_decimal_type( {  2814749UL, 76710656UL },  8 ),
    wide_decimal_type( {  5629499UL, 53421312UL },  8 ),
    wide_decimal_type( { 11258999UL,  6842624UL },  8 ),
    wide_decimal_type( { 22517998UL, 13685248UL },  8 ),
    wide_decimal_type( { 45035996UL, 27370496UL },  8 ),
    wide_decimal_type( { 90071992UL, 54740992UL },  8 ),
    wide_decimal_type( {        1UL, 80143985UL,  9481984UL }, 16 ),
    wide_decimal_type( {        3UL, 60287970UL, 18963968UL }, 16 ),
    wide_decimal_type( {        7UL, 20575940UL, 37927936UL }, 16 ),
    wide_decimal_type( {       14UL, 41151880UL, 75855872UL }, 16 ),
    wide_decimal_type( {       28UL, 82303761UL, 51711744UL }, 16 ),
    wide_decimal_type( {       57UL, 64607523UL,  3423488UL }, 16 ),
    wide_decimal_type( {      115UL, 29215046UL,  6846976UL }, 16 ),
    wide_decimal_type( {      230UL, 58430092UL, 13693952UL }, 16 ),
    wide_decimal_type( {      461UL, 16860184UL, 27387904UL }, 16 ),
    wide_decimal_type( {      922UL, 33720368UL, 54775808UL }, 16 ),
    wide_decimal_type( {     1844UL, 67440737UL,  9551616UL }, 16 ),
    wide_decimal_type( {     3689UL, 34881474UL, 19103232UL }, 16 ),
    wide_decimal_type( {     7378UL, 69762948UL, 38206464UL }, 16 ),
    wide_decimal_type( {    14757UL, 39525896UL, 76412928UL }, 16 ),
    wide_decimal_type( {    29514UL, 79051793UL, 52825856UL }, 16 ),
    wide_decimal_type( {    59029UL, 58103587UL,  5651712UL }, 16 ),
    wide_decimal_type( {   118059UL, 16207174UL, 11303424UL }, 16 ),
    wide_decimal_type( {   236118UL, 32414348UL, 22606848UL }, 16 ),
    wide_decimal_type( {   472236UL, 64828696UL, 45213696UL }, 16 ),
    wide_decimal_type( {   944473UL, 29657392UL, 90427392UL }, 16 ),
    wide_decimal_type( {  1888946UL, 59314785UL, 80854784UL }, 16 ),
    wide_decimal_type( {  3777893UL, 18629571UL, 61709568UL }, 16 ),
    wide_decimal_type( {  7555786UL, 37259143UL, 23419136UL }, 16 ),
    wide_decimal_type( { 15111572UL, 74518286UL, 46838272UL }, 16 ),
    wide_decimal_type( { 30223145UL, 49036572UL, 93676544UL }, 16 ),
    wide_decimal_type( { 60446290UL, 98073145UL, 87353088UL }, 16 ),
    wide_decimal_type( {        1UL, 20892581UL, 96146291UL, 74706176UL }, 24 ),
    wide_decimal_type( {        2UL, 41785163UL, 92292583UL, 49412352UL }, 24 ),
    wide_decimal_type( {        4UL, 83570327UL, 84585166UL, 98824704UL }, 24 ),
    wide_decimal_type( {        9UL, 67140655UL, 69170333UL, 97649408UL }, 24 ),
    wide_decimal_type( {       19UL, 34281311UL, 38340667UL, 95298816UL }, 24 ),
    wide_decimal_type( {       38UL, 68562622UL, 76681335UL, 90597632UL }, 24 ),
    wide_decimal_type( {       77UL, 37125245UL, 53362671UL, 81195264UL }, 24 ),
    wide_decimal_type( {      154UL, 74250491UL,  6725343UL, 62390528UL }, 24 ),
    wide_decimal_type( {      309UL, 48500982UL, 13450687UL, 24781056UL }, 24 ),
    wide_decimal_type( {      618UL, 97001964UL, 26901374UL, 49562112UL }, 24 ),
    wide_decimal_type( {     1237UL, 94003928UL, 53802748UL, 99124224UL }, 24 ),
    wide_decimal_type( {     2475UL, 88007857UL,  7605497UL, 98248448UL }, 24 ),
    wide_decimal_type( {     4951UL, 76015714UL, 15210995UL, 96496896UL }, 24 ),
    wide_decimal_type( {     9903UL, 52031428UL, 30421991UL, 92993792UL }, 24 ),
    wide_decimal_type( {    19807UL,  4062856UL, 60843983UL, 85987584UL }, 24 ),
    wide_decimal_type( {    39614UL,  8125713UL, 21687967UL, 71975168UL }, 24 ),
    wide_decimal_type( {    79228UL, 16251426UL, 43375935UL, 43950336UL }, 24 ),
    wide_decimal_type( {   158456UL, 32502852UL, 86751870UL, 87900672UL }, 24 ),
    wide_decimal_type( {   316912UL, 65005705UL, 73503741UL, 75801344UL }, 24 ),
    wide_decimal_type( {   633825UL, 30011411UL, 47007483UL, 51602688UL }, 24 ),
    wide_decimal_type( {  1267650UL, 60022822UL, 94014967UL,  3205376UL }, 24 ),
    wide_decimal_type( {  2535301UL, 20045645UL, 88029934UL,  6410752UL }, 24 ),
    wide_decimal_type( {  5070602UL, 40091291UL, 76059868UL, 12821504UL }, 24 ),
    wide_decimal_type( { 10141204UL, 80182583UL, 52119736UL, 25643008UL }, 24 ),
    wide_decimal_type( { 20282409UL, 60365167UL,  4239472UL, 51286016UL }, 24 ),
    wide_decimal_type( { 40564819UL, 20730334UL,  8478945UL,  2572032UL }, 24 ),
    wide_decimal_type( { 81129638UL, 41460668UL, 16957890UL,  5144064UL }, 24 ),
    wide_decimal_type( {        1UL, 62259276UL, 82921336UL, 33915780UL, 10288128UL }, 32 ),
    wide_decimal_type( {        3UL, 24518553UL, 65842672UL, 67831560UL, 20576256UL }, 32 ),
    wide_decimal_type( {        6UL, 49037107UL, 31685345UL, 35663120UL, 41152512UL }, 32 ),
    wide_decimal_type( {       12UL, 98074214UL, 63370690UL, 71326240UL, 82305024UL }, 32 ),
    wide_decimal_type( {       25UL, 96148429UL, 26741381UL, 42652481UL, 64610048UL }, 32 ),
    wide_decimal_type( {       51UL, 92296858UL, 53482762UL, 85304963UL, 29220096UL }, 32 ),
    wide_decimal_type( {      103UL, 84593717UL,  6965525UL, 70609926UL, 58440192UL }, 32 ),
    wide_decimal_type( {      207UL, 69187434UL, 13931051UL, 41219853UL, 16880384UL }, 32 ),
    wide_decimal_type( {      415UL, 38374868UL, 27862102UL, 82439706UL, 33760768UL }, 32 ),
    wide_decimal_type( {      830UL, 76749736UL, 55724205UL, 64879412UL, 67521536UL }, 32 ),
    wide_decimal_type( {     1661UL, 53499473UL, 11448411UL, 29758825UL, 35043072UL }, 32 ),
    wide_decimal_type( {     3323UL,  6998946UL, 22896822UL, 59517650UL, 70086144UL }, 32 ),
    wide_decimal_type( {     6646UL, 13997892UL, 45793645UL, 19035301UL, 40172288UL }, 32 ),
    wide_decimal_type( {    13292UL, 27995784UL, 91587290UL, 38070602UL, 80344576UL }, 32 ),
    wide_decimal_type( {    26584UL, 55991569UL, 83174580UL, 76141205UL, 60689152UL }, 32 ),
    wide_decimal_type( {    53169UL, 11983139UL, 66349161UL, 52282411UL, 21378304UL }, 32 ),
    wide_decimal_type( {   106338UL, 23966279UL, 32698323UL,  4564822UL, 42756608UL }, 32 ),
    wide_decimal_type( {   212676UL, 47932558UL, 65396646UL,  9129644UL, 85513216UL }, 32 ),
    wide_decimal_type( {   425352UL, 95865117UL, 30793292UL, 18259289UL, 71026432UL }, 32 ),
    wide_decimal_type( {   850705UL, 91730234UL, 61586584UL, 36518579UL, 42052864UL }, 32 ),
    wide_decimal_type( {  1701411UL, 83460469UL, 23173168UL, 73037158UL, 84105728UL }, 32 ),
  }};

  bool result_is_ok = true;

  const wide_decimal_type local_half(wide_decimal_type(1U) / 2U);
  const wide_decimal_type local_one (1U);
  const wide_decimal_type local_two (2U);

  for(std::size_t i = 0U; i < local_pow2_data.size(); ++i)
  {
    wide_decimal_type x2;

    if(i <= 127U)
    {
      x2 = pow(local_half, std::ptrdiff_t(128 - std::ptrdiff_t(i)));
    }
    else if(i == 128U)
    {
      x2 = local_one;
    }
    else
    {
      x2 = pow(local_two, std::ptrdiff_t(std::ptrdiff_t(i) - 128));
    }

    result_is_ok &= (x2 == local_pow2_data[i]);
  }

  return result_is_ok;
}

// Enable this if you would like to activate this main() as a standalone example.
#if 0

#include <iomanip>
#include <iostream>

int main()
{
  const bool result_is_ok = math::wide_decimal::example001d_pow2_from_list();

  std::cout << "result_is_ok: " << std::boolalpha << result_is_ok << std::endl;
}

#endif
