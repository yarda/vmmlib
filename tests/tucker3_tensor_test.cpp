#include "tucker3_tensor_test.hpp"

#include <vmmlib/tucker3_tensor.hpp>

#include <sstream>

namespace vmml
{
	
	bool
	tucker3_tensor_test::run()
	{
		bool ok = false;
		
		//decomposition (hosvd test data after lathauwer 2000a)
		//prepare control data
		matrix<3, 3, double> u1_hosvd;
		matrix<3, 3, double> u2_hosvd;
		matrix<3, 3, double> u3_hosvd;
		matrix<3, 3, double> u1_hosvd_check;
		matrix<3, 3, double> u2_hosvd_check;
		matrix<3, 3, double> u3_hosvd_check;
		double data_u1_hosvd[] = { 0.1121, -0.7739, -0.6233, 0.5771, 0.5613, -0.5932, 0.8090, -0.2932, 0.5095 };
		u1_hosvd_check.set( data_u1_hosvd, data_u1_hosvd + 9);
		double data_u2_hosvd[] = { 0.4624, 0.0102, 0.8866, 0.8866, -0.0135, -0.4623, -0.0072, -0.9999, 0.0152 };
		u2_hosvd_check.set( data_u2_hosvd, data_u2_hosvd + 9);
		double data_u3_hosvd[] = { 0.6208, -0.4986, 0.6050, -0.0575, -0.7986, -0.5992, 0.7819, 0.3371, -0.5244 };
		u3_hosvd_check.set( data_u3_hosvd, data_u3_hosvd + 9);
		
		tensor3<3, 3, 3, double> core_hosvd;
		tensor3<3, 3, 3, double> core_hosvd_check;
		//original data from paper: 
		/*double data_core_hosvd[] = { 
			8.7088, 0.1066, -0.0033, -0.0256, 3.1965, 0.2948, 0.0, 0.0, 0.0, 
			0.0489, 3.2737, -0.1797, 3.2546, -0.2130, -0.0378, 0.0, 0.0, 0.0, 
			-0.2797,  0.3223,  -0.2222, -0.2853, 0.7829, -0.3704, 0.0, 0.0, 0.0
		};*/
		double data_core_hosvd[] = {8.708472160155013, 0.102998157107751, -0.003323406554955818,
			-0.02516469920654374, 3.224306905801339, 0.2941942573131867,
			0.000191418002237989, 0.02242148039036788, -0.0003651611722031906,
			0.04864077990325821, 3.217043758423341, -0.1789922055828147,
			3.25762887465715, 0.1764987048963867, -0.04376067308594064,
			0.002196066278081278, 0.3135580392355695, -0.004763177812981762,
			-0.2795251130393259, 0.2797854314260668, -0.2216368793455382,
			-0.2834685427810342, 1.075124397867417, -0.374930365623059,
			0.001697013300563536, 0.2351618057118901, -0.003603936260039459};
		double data_core_matlab[] = { 
			8.7092,     0.1024,   -0.0035,
			-0.0257,    3.2247,    0.2945,
			-0.0000,    0.0225,   -0.0004,
			0.0481,    3.2176,   -0.1788,
			3.2572,    0.1762,   -0.0439,
			0.0023,    0.3135,   -0.0047,
			-0.2795,   0.2801,   -0.2217,
			-0.2832,    1.0753,   -0.3748,
			0.0017,    0.2352,   -0.0036,
		};
		core_hosvd_check.set( data_core_matlab, data_core_matlab + 27);

		
		//do decomposition
		tensor3< 3, 3, 3, double> t3_data_hosvd;
		double data_hosvd[] = { 
			0.9073, 1.7842, 2.1236, 0.8924, 1.7753, -0.6631, 2.1488, 4.2495, 1.8260, 
			0.7158, 1.6970, -0.704, -0.4898, -1.5077, 1.9103, 0.3054, 0.3207, 2.1335, 
			-0.3698, 0.0151, 1.4429, 2.4288, 4.0337, -1.7495, 2.3753, 4.7146, -0.2716
		};
		t3_data_hosvd.set(data_hosvd, data_hosvd + 27);
		
		tucker3_tensor< 3, 3, 3, 3, 3, 3, double > tuck3_hosvd( core_hosvd, u1_hosvd, u2_hosvd, u3_hosvd );
		
		//(1) derive core tensor
		tuck3_hosvd.derive_core( t3_data_hosvd, core_hosvd, u1_hosvd_check, u2_hosvd_check, u3_hosvd_check );
		
		if ( core_hosvd.equals( core_hosvd_check, 0.001 ))
		{	
			log( "HOSVD derive core tensor", true  );
		} else
		{
			std::stringstream error;
			error 
			<< "HOSVD derive core: " << std::endl
			<< "core should be: " << std::endl << core_hosvd_check << std::endl
			<< "core is: " << std::setprecision(16) << std::endl << core_hosvd << std::endl;
			
			log_error( error.str() );
		}
		
		
		//(2) decomposition into basis matrices (different basis matrices as in de lathauwer et al. 2000a, but equal to svds in matlab)
		double data_u1_hosvd2[] = {-0.0883627, 0.786186, -0.61164, -0.580928, -0.539474, -0.6095, -0.809145, 0.301462, 0.504387};
		double data_u2_hosvd2[] = {-0.462307, 0.00227505, -0.886717, -0.886361, 0.0272671, 0.462191, 0.0252297, 0.999626, -0.0105893 };
		double data_u3_hosvd2[] = { -0.618101, -0.502086, -0.604864, 0.0794671, -0.805418, 0.587356, -0.782072, 0.314978, 0.537729};
		u1_hosvd_check.set( data_u1_hosvd2, data_u1_hosvd2 + 9);
		u2_hosvd_check.set( data_u2_hosvd2, data_u2_hosvd2 + 9);
		u3_hosvd_check.set( data_u3_hosvd2, data_u3_hosvd2 + 9);
		/*//core computed by own basis matrices
		 double data_core_hosvd[] = {-8.70537, -0.0766941, 0.000824482, 0.0887245, -3.23601, -0.304253, -0.10743, -0.0229932, -0.00532969,
		 0.00306063, 3.22528, -0.167909, 3.25526, 0.00968507, -0.049939, 0.0455181, -0.275464, 0.000384592,
		 0.379315, -0.266205, 0.225664, 0.30135, -1.04242, 0.368141, 0.0155323, 0.210111, 0.0064294};*/
		
		
		tuck3_hosvd.hosvd( t3_data_hosvd );
		u1_hosvd = tuck3_hosvd.get_u1();
		u2_hosvd = tuck3_hosvd.get_u2();
		u3_hosvd = tuck3_hosvd.get_u3();
		
		if ( u1_hosvd.equals( u1_hosvd_check, 0.001 ) && u2_hosvd.equals( u2_hosvd_check, 0.001 ) && u3_hosvd.equals( u3_hosvd_check, 0.001 ))
		{	
			log( "HOSVD compute basis matrices U1, U2, U3", true  );
		} else
		{
			std::stringstream error;
			error 
			<< "HOSVD: " << std::endl
			<< "U1 should be: " << std::endl << u1_hosvd_check << std::endl
			<< "U1 is: " << std::endl << u1_hosvd << std::endl
			<< "U2 should be: " << std::endl << u2_hosvd_check << std::endl
			<< "U2 is: " << std::endl << u2_hosvd << std::endl
			<< "U3 should be: " << std::endl << u3_hosvd_check << std::endl
			<< "U3 is: " << std::endl << u3_hosvd << std::endl;
			
			
			log_error( error.str() );
		}
		
		
		//(3) higher-order orthogonal iteration (hoii test data after lathauwer 2000b)
		matrix<3, 3, double> u1_hoii;
		matrix<2, 2, double> u2_hoii;
		matrix<2, 2, double> u3_hoii;
		matrix<3, 3, double> u1_hoii_check;
		matrix<2, 2, double> u2_hoii_check;
		matrix<2, 2, double> u3_hoii_check;
		double data_u1_hoii_step1[] = {-0.246452, 0.499323, 0.830625, 0.521727, -0.653918, 0.547898, 0.816738, 0.56839, -0.0993512};
		u1_hoii_check.set( data_u1_hoii_step1, data_u1_hoii_step1 + 9);
		double data_u2_hoii_step1[] = {0.171472, 0.985189, 0.985189, -0.171472};
		u2_hoii_check.set( data_u2_hoii_step1, data_u2_hoii_step1 + 4);
		double data_u3_hoii_step1[] = {-0.510464, 0.859899, 0.859899, 0.510464};
		u3_hoii_check.set( data_u3_hoii_step1, data_u3_hoii_step1 + 4);

		tensor3<3, 2, 2, double> core_hoii;
		tensor3< 3, 2, 2, double> t3_data_hoii;
		double data_hoii[] = { 0, 1, 2, 3, 4, 5, -1, 4, -2, -5, 3, -6};
		t3_data_hoii.set(data_hoii, data_hoii + 12);
		
		tucker3_tensor< 3, 2, 2, 3, 2, 2, double > tuck3_hoii( core_hoii, u1_hoii, u2_hoii, u3_hoii );
		//Step 1
		tuck3_hoii.hosvd( t3_data_hoii );
		u1_hoii = tuck3_hoii.get_u1();
		u2_hoii = tuck3_hoii.get_u2();
		u3_hoii = tuck3_hoii.get_u3();
		core_hoii = tuck3_hoii.get_core();
		
		if ( u1_hoii.equals( u1_hoii_check, 0.001 ) && u2_hoii.equals( u2_hoii_check, 0.001 ) && u3_hoii.equals( u3_hoii_check, 0.001 ))
		{	
			log( "HOII (step 1) initalize basis matrices by HOSVD U1, U2, U3", true  );
		} else
		{
			std::stringstream error;
			error 
			<< "HOII (step 1): " << std::endl
			<< "U1 should be: " << std::endl << u1_hoii_check << std::endl
			<< "U1 is: " << std::endl << u1_hoii << std::endl
			<< "U2 should be: " << std::endl << u2_hoii_check << std::endl
			<< "U2 is: " << std::endl << u2_hoii << std::endl
			<< "U3 should be: " << std::endl << u3_hoii_check << std::endl
			<< "U3 is: " << std::endl << u3_hoii << std::endl;
			
			
			log_error( error.str() );
		}
		
		//Step 2
		
		matrix<3, 2, double> u1_hoii_2;
		matrix<2, 2, double> u2_hoii_2;
		matrix<2, 1, double> u3_hoii_2;
		matrix<3, 2, double> u1_hoii_check_2;
		matrix<2, 2, double> u2_hoii_check_2;
		matrix<2, 1, double> u3_hoii_check_2;
		double data_u1_hoii_step2[] = {-0.2789474111071825, -0.4141266306147141, 0.5983607967045261, -0.7806355076145298, 0.7511009910815755, 0.4680890279285662}; //original from paper (u1): {-0.2789, -0.4141, 0.5984, -0.7806, 0.7511, 0.4681};
		u1_hoii_check_2.set( data_u1_hoii_step2, data_u1_hoii_step2 + 6);
		double data_u2_hoii_step2[] = {0.09816424894941822, 0.9951702267593203, 0.9951702267593203, -0.09816424894941811};//original in paper (u2): 0.0982, -0.9952, 0.9952, 0.0982};
		u2_hoii_check_2.set( data_u2_hoii_step2, data_u2_hoii_step2 + 4);
		double data_u3_hoii_step2[] = {-0.5104644303570166, 0.8598988692516618};//original in paper (u3): {0.5105, -0.8599};
		u3_hoii_check_2.set( data_u3_hoii_step2, data_u3_hoii_step2 + 2);
		
		tensor3<2, 2, 1, double> core_hoii_2;
		tensor3<2, 2, 1, double> core_hoii_check_2;
		double data_core_hoii_2[] = { -10.14733447424582, 0.0, 0.0, 2.760705584847321 };
		core_hoii_check_2.set( data_core_hoii_2, data_core_hoii_2 + 4);
		
		tucker3_tensor< 2, 2, 1, 3, 2, 2, double > tuck3_hoii_2( core_hoii_2, u1_hoii_2, u2_hoii_2, u3_hoii_2 );
		
		tuck3_hoii_2.hoii( t3_data_hoii );
		u1_hoii_2 = tuck3_hoii_2.get_u1();
		u2_hoii_2 = tuck3_hoii_2.get_u2();
		u3_hoii_2 = tuck3_hoii_2.get_u3();
		core_hoii_2 = tuck3_hoii_2.get_core();
		
		if ( u1_hoii_2.equals( u1_hoii_check_2, 0.001 ) && u2_hoii_2.equals( u2_hoii_check_2, 0.001 ) && u3_hoii_2.equals( u3_hoii_check_2, 0.001 ) && core_hoii_2.equals( core_hoii_check_2, 0.001))
		{	
			log( "HOII (step 2) rank-(2,2,1) approximation" , true  );
		} else
		{
			std::stringstream error;
			error 
			<< "HOII (step 2) rank-(2,2,1) approximation: " << std::setprecision(16) << std::endl
			<< "U1 should be: " << std::endl << u1_hoii_check_2 << std::endl
			<< "U1 is: " << std::endl << u1_hoii_2 << std::endl
			<< "U2 should be: " << std::endl << u2_hoii_check_2 << std::endl
			<< "U2 is: " << std::endl << u2_hoii_2 << std::endl
			<< "U3 should be: " << std::endl << u3_hoii_check_2 << std::endl
			<< "U3 is: " << std::endl << u3_hoii_2 << std::endl
			<< "core should be: " << std::endl << core_hoii_check_2 << std::endl
			<< "core is: " << std::endl << core_hoii_2 << std::endl;
			
			
			log_error( error.str() );
		}
		
		
		//tucker3 reconstruction
		tensor3< 2, 3, 4, uint16_t >  core;
		core.fill_increasing_values();
		tensor3< 6, 7, 5, uint16_t >  t3_reco;
		matrix<6, 2, uint16_t> u1;
		matrix<7, 3, uint16_t> u2;
		matrix<5, 4, uint16_t> u3;
		u1.fill(2);
		u2.fill(3);
		u3.fill(1);
		
		tucker3_tensor<2, 3, 4, 6, 7, 5, uint16_t > tuck3( core, u1, u2, u3 );

		tuck3.reconstruction( t3_reco );
		
		tensor3<6, 7, 5, uint16_t> t3_reco_test;
		t3_reco_test.fill(1656);
		//std::cout << "Tucker3 reconstruction (all values should be 1656): " << std::endl << t3_reco << std::endl;
		//std::cout << "Tucker3 core : " << std::endl << tuck3.get_core() << std::endl;
		
		if ( t3_reco_test == t3_reco)
		{	
			log( "tucker3 reconstruction", true  );
		} else
		{
			std::stringstream error;
			error 
			<< "Tucker3 reconstruction (all values should be 1656): " << std::endl << t3_reco
			<< std::endl;
			log_error( error.str() );
		}
		

		//rank reduction
		tensor3< 1, 2, 3, uint16_t >  core_red;
		matrix<6, 1, uint16_t> u1_red;
		matrix<7, 2, uint16_t> u2_red;
		matrix<5, 3, uint16_t> u3_red;
		
		tucker3_tensor< 1, 2, 3, 6, 7, 5, uint16_t > tuck3_red( core_red, u1_red, u2_red, u3_red );
		tuck3_red.reduce_ranks( tuck3 );
		
		u1_red.fill(2);
		u2_red.fill(3);
		u3_red.fill(1);
		double data[] = { 0, 1, 6, 7, 12, 13 };
		core_red.set(data, data+6);
		
		if ( tuck3_red.get_u1() == u1_red && tuck3_red.get_u2() == u2_red && tuck3_red.get_u3() == u3_red && tuck3_red.get_core() == core_red)
		{	
			log( "tucker3 reduce ranks", true  );
		} else
		{
			std::stringstream error;
			error 
			<< "Tucker3 reduce ranks: " << std::endl
			<< "u1 should be: " << u1_red << std::endl
			<< "u1 is: " << tuck3_red.get_u1() << std::endl
			<< "u2 should be: " <<  u2_red << std::endl
			<< "u2 is: " <<  tuck3_red.get_u2() << std::endl
			<< "u3 should be: " << u3_red << std::endl
			<< "u3 is: " << tuck3_red.get_u3() << std::endl
			<< "core should be: " << core_red << std::endl
			<< "core is: " << tuck3_red.get_core() << std::endl;

			log_error( error.str() );
		}
		
		
		//basis matrices subsampling
		tensor3< 3, 4, 3, uint16_t > t3_sub;
		tensor3< 2, 3, 4, uint16_t > core_sub;
		matrix< 3, 2, uint16_t > u1_sub;
		matrix< 4, 3, uint16_t > u2_sub;
		matrix< 3, 4, uint16_t > u3_sub;
		tucker3_tensor< 2, 3, 4, 3, 4, 3, uint16_t > tuck3_sub( core_sub, u1_sub, u2_sub, u3_sub );
		
		tuck3_sub.subsampling( tuck3, 2);
		tuck3_sub.reconstruction( t3_sub );
		
		tensor3< 3, 4, 3, uint16_t > t3_sub_test;
		t3_sub_test.fill(1656);
		if ( t3_sub_test == t3_sub )
		{	
			log( "basis matrices subsampling", true  );
		} else
		{
			std::stringstream error;
			error 
			<< "basis matrices subsampling with factor 2: " << std::endl << t3_sub
			<< std::endl;
			log_error( error.str() );
		}
		
		//basis matrices subsampling, average data
		tensor3< 3, 4, 3, uint16_t > t3_sub_avg;
		tensor3< 2, 3, 4, uint16_t > core_sub_avg;
		matrix< 3, 2, uint16_t > u1_sub_avg;
		matrix< 4, 3, uint16_t > u2_sub_avg;
		matrix< 3, 4, uint16_t > u3_sub_avg;
		tucker3_tensor< 2, 3, 4, 3, 4, 3, uint16_t > tuck3_sub_avg( core_sub_avg, u1_sub_avg, u2_sub_avg, u3_sub_avg );
		
		/*matrix<6, 2, uint16_t> u1_new;
		u1_new.fill(5);
		u1_new.at(1,1) = 2;
		u1_new.at(4,0) = 3;
		u1_new.at(3,1) = 8;
		tuck3.set_u1(u1_new);*/
		
		tuck3_sub_avg.subsampling_on_average( tuck3, 2);
		tuck3_sub_avg.reconstruction( t3_sub_avg );
		
		t3_sub_test.fill(1656);
		if ( t3_sub_test == t3_sub_avg )
		{	
			log( "basis matrices subsampling on average", true  );
		} else
		{
			std::stringstream error;
			error 
			<< "basis matrices subsampling on average with factor 2: " << std::endl << t3_sub_avg
			<< std::endl;
			log_error( error.str() );
		}
		
		
		//basis matrices region of interest selection
		tensor3< 1, 1, 3, uint16_t > t3_roi;
		tensor3< 2, 3, 4, uint16_t > core_roi;
		matrix< 1, 2, uint16_t > u1_roi;
		matrix< 1, 3, uint16_t > u2_roi;
		matrix< 3, 4, uint16_t > u3_roi;
		tucker3_tensor< 2, 3, 4, 1, 1, 3, uint16_t > tuck3_roi( core_roi, u1_roi, u2_roi, u3_roi );
		
		tuck3_roi.region_of_interest( tuck3, 0, 1, 1, 2, 1, 4);
		tuck3_roi.reconstruction( t3_roi );

		tensor3< 1, 1, 3, uint16_t > t3_roi_test;
		t3_roi_test.fill(1656);
		if ( t3_roi_test == t3_roi)
		{	
			log( "basis matrices region of interest selection", true  );
		} else
		{
			std::stringstream error;
			error 
			<< "basis matrices region of interest selection: " << std::endl << t3_roi
			<< std::endl;
			log_error( error.str() );
		}
		
		
	
		ok = true;
		return ok;
	}
	
	
	
} // namespace vmml
