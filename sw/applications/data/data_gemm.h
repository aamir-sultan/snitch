#include "layer.h"

layer gemm_l = {
	.type = GEMM,
	.M = 16,
	.N = 16,
	.K = 16,
	.TA = 0,
	.TB = 1,
	.ALPHA = 0,
	.dtype = FP16
};


static __fp16 gemm_A_dram [16][16] = {1.9269152879714966, 1.4872840642929077, 0.9007171988487244, -2.1055209636688232, 0.6784184575080872, -1.2345448732376099, -0.04306747764348984, -1.6046669483184814, -0.7521352767944336, 1.6487230062484741, -0.3924786448478699, -1.4036071300506592, -0.7278813123703003, -0.5594301819801331, -0.7688388824462891, 0.7624453902244568, 1.6423169374465942, -0.1595974713563919, -0.4973975419998169, 0.439589262008667, -0.7581311464309692, 1.078317642211914, 0.8008005619049072, 1.680620551109314, 1.27912437915802, 1.2964228391647339, 0.610466480255127, 1.334737777709961, -0.2316243201494217, 0.041759490966796875, -0.2515752911567688, 0.859858512878418, -1.3846737146377563, -0.8712361454963684, -0.223365917801857, 1.7173614501953125, 0.3188803195953369, -0.42451897263526917, 0.3057209253311157, -0.7745925188064575, -1.5575724840164185, 0.9956361055374146, -0.8797858357429504, -0.6011420488357544, -1.2741512060165405, 2.1227850914001465, -1.234653115272522, -0.4879138767719269, -0.9138230085372925, -0.6581372618675232, 0.07802387326955795, 0.5258087515830994, -0.48799172043800354, 1.1913690567016602, -0.8140076398849487, -0.7359927892684937, -1.4032478332519531, 0.03600366786122322, -0.06347727030515671, 0.6756148934364319, -0.0978068932890892, 1.8445940017700195, -1.184537410736084, 1.3835493326187134, 1.4451338052749634, 0.8564125299453735, 2.218075752258301, 0.5231655240058899, 0.34664666652679443, -0.19733144342899323, -1.0545889139175415, 1.2779955863952637, -0.1721901297569275, 0.5237884521484375, 0.056621819734573364, 0.4262961447238922, 0.575005054473877, -0.6417241096496582, -2.2063984870910645, -0.7508030533790588, 0.01086814422160387, -0.33874234557151794, -1.3406795263290405, -0.5853705406188965, 0.5361881256103516, 0.5246226191520691, 1.1412016153335571, 0.05164359509944916, 0.7439519762992859, -0.4815843999385834, -1.0494661331176758, 0.603898823261261, -1.7222950458526611, -0.827768862247467, 1.334702968597412, 0.48353928327560425, -2.5095443725585938, 0.4880010485649109, 0.7845868468284607, 0.02864718623459339, 0.640755295753479, 0.5832474231719971, 1.0669267177581787, -0.4501533806324005, -0.18526747822761536, 0.7527588605880737, 0.4047577977180481, 0.17846599221229553, 0.2649095058441162, 1.2731683254241943, -0.0013108636485412717, -0.30360376834869385, -1.457029104232788, -0.10233523696660995, -0.5991530418395996, 0.4770564138889313, 0.7261772155761719, 0.09115186333656311, -0.3890652060508728, 0.5279164910316467, -0.012685478664934635, 0.24083632230758667, 0.13253536820411682, 0.7642406225204468, 1.095009684562683, 0.3398909568786621, 0.7199674844741821, 0.41140761971473694, 1.931160569190979, 1.0118638277053833, -1.4364064931869507, -1.1298598051071167, -0.1360345333814621, 1.6354095935821533, 0.6547407507896423, 0.5760045647621155, 1.1415079832077026, 0.018564576283097267, -1.8058050870895386, 0.9254348874092102, -0.3753443658351898, 1.0330873727798462, -0.6866509318351746, 0.6368136405944824, -0.9726738929748535, 0.9584577679634094, 1.6192004680633545, 1.450609803199768, 0.2694815397262573, -0.21037597954273224, -0.7328027486801147, 0.10429783165454865, 0.3487516939640045, 0.9675941467285156, -0.46568843722343445, 1.6047972440719604, -2.4801201820373535, -0.4175437390804291, -1.1954537630081177, 0.8123369216918945, -1.9005532264709473, 0.22857652604579926, 0.02485940419137478, -0.34595024585723877, 0.2868328094482422, -0.7308424115180969, 0.17482025921344757, -1.0939292907714844, -1.6021603345870972, 1.3528969287872314, 1.288827657699585, 0.05229547247290611, -1.5468504428863525, 0.7567060589790344, 0.7755194902420044, 2.0265355110168457, 0.03581761196255684, 0.12058872729539871, -0.8056637048721313, -0.20757682621479034, -0.9319478273391724, -1.5909662246704102, -1.13597571849823, -0.52259761095047, -0.5187733173370361, -1.5012763738632202, -1.9266542196273804, 0.1278512328863144, 1.0229133367538452, -0.5557951331138611, 0.7042727470397949, 0.7098760008811951, 1.7743884325027466, -0.921550989151001, 0.9624499082565308, -0.33701515197753906, -1.1753336191177368, 0.35805708169937134, 0.47876790165901184, 1.353700041770935, 0.5260620713233948, 2.1120378971099854, -0.5207571387290955, -0.9320061206817627, 0.18516133725643158, 1.0686918497085571, 1.3065344095230103, 0.4598345160484314, -0.8146268725395203, -1.0212392807006836, -0.49492356181144714, -0.5922516584396362, 0.15431594848632812, 0.4407670795917511, -0.14829230308532715, -2.3184432983398438, -0.39799532294273376, 1.0804862976074219, -1.7808643579483032, 1.5080454349517822, 0.30942854285240173, -0.5003090500831604, 1.0350031852722168, 1.6896470785140991, -0.004505051765590906, 1.666792392730713, 0.15392017364501953, -1.0602530241012573, -0.572657585144043, 0.0835680365562439, 0.39990535378456116, 1.989207148551941, -0.07198750972747803, -0.906094491481781, -2.0487122535705566, -1.0810555219650269, 0.01762307994067669, 0.0782259851694107, 0.19315829873085022, 0.40967342257499695, -0.9291303157806396, 0.2761908769607544, -0.5388752818107605, 0.4625823199748993, -0.8718891143798828, -0.027118360623717308, -0.3532457649707794, 1.4638569355010986, 1.255434274673462, -0.7149558067321777, 0.8539193272590637, 0.512991189956665, 0.5397310256958008, 0.5655050277709961, 0.5057917237281799, 0.22245365381240845};


static __fp16 gemm_B_dram [16][16] = {-0.685481607913971, -1.3793021440505981, 0.9732939004898071, 0.4038029611110687, 0.4334380030632019, 0.8853808045387268, -0.7939430475234985, -0.21844321489334106, 0.6663737893104553, 0.024699924513697624, 0.6838752627372742, -0.4939993619918823, -1.3838845491409302, 1.096641182899475, 1.621694564819336, 1.179942011833191, 0.5635589957237244, 0.625802755355835, -1.0150787830352783, -0.7139783501625061, -0.7171905636787415, 0.1824439913034439, 0.3752319812774658, 0.16629981994628906, -0.07426456362009048, -1.0641486644744873, -1.3245893716812134, 1.1366126537322998, 0.4868715703487396, -0.6836934685707092, 0.8512656688690186, -0.33142781257629395, -1.507175087928772, -2.5849502086639404, -0.5419175624847412, 0.8337291479110718, 1.055369257926941, 0.7863810062408447, 0.08790969103574753, 2.1441681385040283, -0.20960482954978943, -0.7601934671401978, -0.5160817503929138, -0.46184006333351135, -1.0020296573638916, 0.06604336202144623, -0.40046969056129456, 0.6495042443275452, -1.610666036605835, -0.02399955503642559, -0.44102486968040466, -0.9585452079772949, -1.4533969163894653, -0.057920295745134354, -1.241483449935913, 1.7045671939849854, 0.16632132232189178, -0.4075061082839966, 0.6001842617988586, 1.419979453086853, 0.0329488180577755, -0.0007737990817986429, -0.6088271737098694, 0.09495851397514343, -1.4790465831756592, -0.1221928745508194, -0.3136177957057953, 0.45363426208496094, 0.46515071392059326, 0.5666652917861938, -0.32025346159935, 0.3459012508392334, 1.4703037738800049, 0.9623646140098572, -0.4702208340167999, 0.848518967628479, -0.42919591069221497, 0.1620604395866394, -0.508095383644104, -0.7525874376296997, 0.4322742819786072, -0.7469954490661621, -0.12925422191619873, 1.2460919618606567, 0.37139150500297546, -0.7097623348236084, -0.844377875328064, 0.6424751281738281, -0.9390866756439209, -0.14264194667339325, -0.6086435317993164, -0.047891248017549515, -0.9817978739738464, 1.195958137512207, -0.6184902191162109, -0.647229790687561, -0.1250254064798355, 1.7093087434768677, -0.7149624228477478, -2.3065085411071777, -0.004656785633414984, -0.4875054359436035, -0.5513465404510498, -0.20395424962043762, -0.6013189554214478, 0.15271379053592682, -0.046192023903131485, 0.6685602068901062, -0.6420586109161377, -1.3061535358428955, -1.647040605545044, -1.2822614908218384, 0.7821183800697327, 0.05792269483208656, -0.047562163323163986, -1.2868918180465698, 0.07954943925142288, 0.050095997750759125, 1.9889612197875977, 0.6853673458099365, -0.09964022785425186, -0.0388023778796196, -1.6457397937774658, 1.0429801940917969, 0.8265887498855591, -1.4039719104766846, -1.0362098217010498, 1.96529221534729, -1.598767638206482, 1.1929805278778076, 2.0207436084747314, 0.17988650500774384, 0.3781784772872925, 0.6084084510803223, 1.900311827659607, -0.13968797028064728, -0.9851518273353577, 0.9446058869361877, -0.4833274185657501, 0.6899017095565796, 1.591395616531372, -1.0597201585769653, -0.4503057301044464, -0.9638485312461853, -0.10912995040416718, 1.9372931718826294, 0.25391900539398193, -2.126762628555298, 0.7051141262054443, 1.6308681964874268, 1.6950805187225342, -1.1807502508163452, -2.488459348678589, -1.5824053287506104, -0.740294337272644, -1.3129348754882812, -0.1208132952451706, 0.3057299852371216, -0.0729660615324974, -2.5667941570281982, 0.7151994705200195, 0.7287133932113647, 0.9364385008811951, -0.13408313691616058, -1.7236974239349365, -0.08472306281328201, 0.028089528903365135, -1.282929539680481, -0.33131900429725647, 0.9871290922164917, 0.31428107619285583, 0.03780364990234375, -0.48302069306373596, 0.4150581359863281, -0.5479549169540405, 0.7096128463745117, 0.03913922235369682, 0.9808937907218933, 0.7122363448143005, -1.0407685041427612, -0.8434810638427734, 1.0844124555587769, -0.17536965012550354, 0.448485791683197, 0.8435799479484558, 1.1456739902496338, 0.1415553092956543, -1.1702114343643188, 0.11329790204763412, -0.7174144983291626, -1.1425532102584839, 0.8198426961898804, 1.305860161781311, 0.41459226608276367, -0.031765542924404144, -0.7647228837013245, 0.4351435601711273, 0.9477656483650208, -1.7734959125518799, -0.590737521648407, 0.9874473810195923, -0.14181147515773773, 1.0348176956176758, -0.10318559408187866, 0.0771508663892746, 2.833967924118042, -0.44875210523605347, 0.6214459538459778, 0.24659274518489838, 1.15656316280365, 0.10164086520671844, -0.05528254434466362, 0.26588720083236694, -0.676629364490509, -0.7046414017677307, 0.8540631532669067, -0.33197471499443054, -0.2763414680957794, -0.626437783241272, 1.1894739866256714, -0.9228128790855408, 1.9534740447998047, -0.03045438416302204, 0.42318588495254517, -1.9775909185409546, 0.2690545618534088, 1.3433040380477905, 1.204850673675537, -0.5870985388755798, -0.5730168223381042, -0.39465200901031494, -0.4900680184364319, -0.8076189756393433, -0.19321373105049133, -0.5150922536849976, 0.7606944441795349, -1.2619991302490234, 2.0486814975738525, 0.3830311596393585, -0.33889833092689514, 0.01789604313671589, -0.036629438400268555, 0.7132695913314819, -0.982473611831665, 0.0826888456940651, -0.3303174376487732, 1.8868111371994019, -0.35945725440979004, 0.824364185333252, 0.7767809629440308, 0.6902899742126465, -0.7463049292564392, 1.0860532522201538, -1.0880382061004639, -0.04476971551775932, 0.5179733633995056};


static __fp16 gemm_C_dram [16][16] = {-1.363769769668579, 0.1929578185081482, -0.6103342771530151, 0.16323445737361908, 1.51017165184021, 0.21230429410934448, -0.7252011299133301, -0.9527732729911804, 0.5216943025588989, -0.46386733651161194, 0.18237744271755219, -0.38666075468063354, -1.7906768321990967, 0.09329313784837723, -1.9152568578720093, -0.6421752572059631, 1.3438509702682495, -1.2922308444976807, 0.766244113445282, 0.64540034532547, 0.353316068649292, -2.6474881172180176, -1.4575366973876953, -0.9712379574775696, 0.25403109192848206, -0.1790592074394226, 1.1992844343185425, -0.4292171895503998, 1.010284185409546, 0.6110401153564453, 1.2208385467529297, -0.6076440215110779, -1.7376028299331665, -0.12535162270069122, -1.3658148050308228, 1.111746072769165, -0.6227966547012329, -0.7891808748245239, -0.167823925614357, 1.6433145999908447, 2.0070879459381104, -1.2531019449234009, 1.118869423866272, 1.7732776403427124, -2.071660280227661, -0.4125255346298218, -0.9769555926322937, -0.03363388776779175, 1.8594977855682373, 2.6221468448638916, 0.36905255913734436, 0.3802972435951233, 0.19898031651973724, -0.23609064519405365, 0.30340856313705444, -0.45007675886154175, 0.47390419244766235, 0.6503364443778992, 1.1662380695343018, 0.01693599671125412, 0.5325868129730225, -0.6035352349281311, -0.1742597371339798, 0.6092063784599304, -0.8032152652740479, -1.1209005117416382, 0.1956406533718109, -0.7815181016921997, -1.7898789644241333, -0.26157355308532715, -0.44025033712387085, 2.1848294734954834, -0.48009708523750305, -1.2871733903884888, 0.7388824224472046, 0.03389474004507065, -0.31229403614997864, -0.2541753351688385, -1.205536127090454, -0.9542103409767151, 0.061276569962501526, 0.08526104688644409, 0.7481252551078796, -0.16356196999549866, -0.9085567593574524, 0.3129958212375641, 0.8050477504730225, -1.1133604049682617, 0.4981626570224762, -1.1999552249908447, 0.12711313366889954, 0.4403660297393799, 0.6377718448638916, 0.15978877246379852, 1.7697970867156982, 0.6268176436424255, -1.8736529350280762, 2.3259060382843018, -0.9203909635543823, 0.6661149263381958, -0.44026491045951843, -2.3179564476013184, 1.294582724571228, 0.22267311811447144, -0.8483412265777588, 1.6489421129226685, 1.6005686521530151, -0.07858924567699432, 0.43104586005210876, 0.3683530390262604, 0.7637977004051208, 1.1792222261428833, -0.4137862026691437, 0.5184088349342346, -0.7015367746353149, -0.4323408901691437, 0.1414770483970642, 0.07110362499952316, 0.5633530616760254, -0.5786357522010803, -1.083811640739441, -0.3889259994029999, 0.8126105666160583, 1.4981187582015991, 0.043896086513996124, 1.4443233013153076, 0.23202891647815704, 0.5064983367919922, -1.2786966562271118, -0.03842746838927269, 1.9138009548187256, 0.3378446102142334, 0.12505611777305603, -0.7621514797210693, -1.190559983253479, 0.7756073474884033, 0.455719918012619, 0.2503303289413452, -1.3610970973968506, 1.8018341064453125, -0.07434194535017014, -0.15664155781269073, -0.8708454966545105, -0.6410972476005554, -0.414562851190567, -0.6902380585670471, -0.22995619475841522, -2.172283887863159, 0.08768323808908463, 1.0937845706939697, -0.1177205815911293, -0.29864323139190674, -0.9536206126213074, -0.09247277677059174, -1.01665461063385, -0.007675689645111561, -0.518220841884613, 0.83954256772995, 0.05852266773581505, -1.6682480573654175, 2.129624843597412, -1.5181471109390259, 0.1387282907962799, -1.1797568798065186, -0.5297411680221558, 0.9625157713890076, 0.2794382870197296, -0.5718191266059875, -2.7936289310455322, -0.7111545205116272, 0.5235219597816467, -1.71055006980896, 0.8384853601455688, -0.2698453664779663, 0.12306158244609833, 0.8757511377334595, 0.15132997930049896, 0.739393413066864, 0.27310314774513245, 2.7312309741973877, 0.43200522661209106, -0.30918216705322266, -0.09658124297857285, 1.541925072669983, -0.108744777739048, -0.4189043343067169, 1.4384385347366333, -0.7068426609039307, -1.2519514560699463, 3.0250484943389893, 1.3462589979171753, 0.8556069731712341, 0.3220294117927551, 0.44605663418769836, 1.5229592323303223, 1.2804899215698242, -0.11616043001413345, 1.3705363273620605, -0.4809381365776062, -0.9903622269630432, -1.3641812801361084, 0.008205652236938477, -0.40586018562316895, -0.7110859751701355, -0.3495793640613556, 0.3797488212585449, 0.9993040561676025, 1.2751853466033936, 0.9594927430152893, 0.10350999981164932, 0.8290349841117859, 2.0921294689178467, 0.7953095436096191, 0.2792847752571106, 0.1864478439092636, 0.3547132909297943, 0.09063850343227386, 1.7422553300857544, -1.2660012245178223, 0.38916081190109253, 0.34287506341934204, -1.4590637683868408, -1.4936561584472656, -0.22138521075248718, 0.22523505985736847, -0.07724537700414658, 0.9856945276260376, 1.2783364057540894, 0.28815189003944397, 0.869049608707428, -0.8097057938575745, -1.4298604726791382, 0.45901596546173096, 0.5309328436851501, -1.3614802360534668, 1.9562491178512573, 1.7684898376464844, -0.9857985377311707, -1.2370758056640625, -2.301875114440918, -0.0010087001137435436, -0.8494256734848022, -1.6593921184539795, 0.3062905967235565, 1.182044506072998, 0.32602694630622864, -0.3894469738006592, 2.8543806076049805, 0.8243650794029236, 0.7983470559120178, 1.8890222311019897, 0.5934628248214722, 0.0696544423699379, -1.6034338474273682, -0.42982181906700134, 0.5761587619781494};


static __fp16 gemm_result[16][16] __attribute__((section(".data")));

static __fp16 gemm_checksum[16] = {5.58123779296875, -0.6667671203613281, 10.539097785949707, 19.488296508789062, 10.47787857055664, -0.18538665771484375, 22.92155647277832, -0.9203616380691528, 20.284116744995117, 23.065210342407227, 10.313627243041992, -10.060399055480957, -4.560687065124512, 2.089163303375244, 23.02264404296875, -0.68312668800354};


