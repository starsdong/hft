

#ifndef STREADHFTTREES_H
#define STREADHFTTREES_H

//----------------------------------------------------------------------------------------
void Draw_STAR_3D()
{

    TGeoManager *geom = new TGeoManager("geom","My 3D Project");
    //------------------Creat materials------------------------------
    TGeoMaterial *vacuum = new TGeoMaterial("vacuum",0,0,0);
    TGeoMaterial *Fe = new TGeoMaterial("Fe",55.84,26.7,7.87);
    Fe->SetTransparency(80); // higher value means more transparent, 100 is maximum

    TGeoMaterial *M_outer_tube = new TGeoMaterial("M_outer_tube",55.84,26.7,7.87);
    M_outer_tube->SetTransparency(93); // higher value means more transparent, 100 is maximum

    TGeoMaterial *M_IDS = new TGeoMaterial("M_IDS",55.84,26.7,7.87);
    M_IDS       ->SetTransparency(80); // higher value means more transparent, 100 is maximum

    TGeoMaterial *M_beampipe = new TGeoMaterial("M_beampipe",55.84,26.7,7.87);
    M_beampipe       ->SetTransparency(70); // higher value means more transparent, 100 is maximum

    TGeoMaterial *M_Pixel_support = new TGeoMaterial("M_Pixel_support",55.84,26.7,7.87);
    M_Pixel_support    ->SetTransparency(70); // higher value means more transparent, 100 is maximum


    //------------------Create media---------------------------------
    TGeoMedium *Air = new TGeoMedium("Air",0,vacuum);
    TGeoMedium *Iron = new TGeoMedium("Iron",1,Fe);
    TGeoMedium *Me_outer_tube = new TGeoMedium("Me_outer_tube",1,M_outer_tube);
    TGeoMedium *Me_IDS        = new TGeoMedium("Me_IDS",1,M_IDS);
    TGeoMedium *Me_beampipe   = new TGeoMedium("Me_beampipe",1,M_beampipe);
    TGeoMedium *Me_Pixel_support   = new TGeoMedium("Me_Pixel_support",1,M_Pixel_support);

    //------------------Create TOP volume----------------------------
    TGeoVolume *top = geom->MakeBox("top",Air,500,500,500);
    geom->SetTopVolume(top);
    geom->SetTopVisible(0);
    // If you want to see the boundary, please input the number, 1 instead of 0.
    // Like this, geom->SetTopVisible(1);


    TGeoVolume *inner_field_tube       = geom->MakeTube("inner_field_tube",Iron,49.5,50.0,200.0);  // r_min, r_max, dz (half of total length)
    TGeoVolume *outer_field_tube       = geom->MakeTube("outer_field_tube",Me_outer_tube,199.5,200.0,200.0);  // r_min, r_max, dz (half of total length)
    TGeoVolume *IDS_central_part       = geom->MakeTube("IDS_central_part",Me_IDS,42.8/2.0,43.0/2.0,56.0);  // r_min, r_max, dz (half of total length)
    TGeoVolume *IDS_side_parts         = geom->MakeTube("IDS_side_parts",Me_IDS,79.3/2.0,79.5/2.0,(222.7-64.0)/2.0);  // r_min, r_max, dz (half of total length)
    TGeoVolume *IDS_connection_parts_R = geom->MakeCone("IDS_connection_parts_R",Me_IDS,(64.0-56.0)/2.0,42.8/2.0,43.0/2.0,79.3/2.0,79.5/2.0); // dz (half of total length), r1_min, r1_max, r2_min, r2_max
    TGeoVolume *IDS_connection_parts_L = geom->MakeCone("IDS_connection_parts_L",Me_IDS,(64.0-56.0)/2.0,79.3/2.0,79.5/2.0,42.8/2.0,43.0/2.0); // dz (half of total length), r1_min, r1_max, r2_min, r2_max

    TGeoVolume *beampipe_central_part       = geom->MakeTube("beampipe_central_part",Me_beampipe,4.05/2.0,4.15/2.0,141.5);  // r_min, r_max, dz (half of total length)
    TGeoVolume *beampipe_side_parts         = geom->MakeTube("beampipe_side_parts",Me_beampipe,9.52/2.0,9.62/2.0,100.0);  // r_min, r_max, dz (half of total length)
    TGeoVolume *beampipe_connection_parts_R = geom->MakeCone("beampipe_connection_parts_R",Me_beampipe,(191.5-141.5)/2.0,4.05/2.0,4.15/2.0,9.52/2.0,9.62/2.0); // dz (half of total length), r1_min, r1_max, r2_min, r2_max
    TGeoVolume *beampipe_connection_parts_L = geom->MakeCone("beampipe_connection_parts_L",Me_beampipe,(191.5-141.5)/2.0,9.52/2.0,9.62/2.0,4.05/2.0,4.15/2.0); // dz (half of total length), r1_min, r1_max, r2_min, r2_max

    TGeoVolume *Pixel_support       = geom->MakeTube("Pixel_support",Me_Pixel_support,21.8/2.0,22.0/2.0,56.0);  // r_min, r_max, dz (half of total length)

    inner_field_tube       ->SetLineColor(4);
    outer_field_tube       ->SetLineColor(kRed-8);
    IDS_central_part       ->SetLineColor(2);  // Inner Detector Support (IDS)
    IDS_side_parts         ->SetLineColor(2);  // Inner Detector Support (IDS)
    IDS_connection_parts_R ->SetLineColor(2);  // Inner Detector Support (IDS)
    IDS_connection_parts_L ->SetLineColor(2);  // Inner Detector Support (IDS)

    beampipe_central_part       ->SetLineColor(3);  // (beampipe)
    beampipe_side_parts         ->SetLineColor(3);  // (beampipe)
    beampipe_connection_parts_R ->SetLineColor(3);  // (beampipe)
    beampipe_connection_parts_L ->SetLineColor(3);  // (beampipe)

    Pixel_support ->SetLineColor(kYellow-3);  // (pixel support)

    top->AddNodeOverlap(inner_field_tube,1,new TGeoTranslation(0,0,0));
    top->AddNodeOverlap(outer_field_tube,1,new TGeoTranslation(0,0,0));
    top->AddNodeOverlap(IDS_central_part,1,new TGeoTranslation(0,0,0));
    top->AddNodeOverlap(IDS_side_parts,1,new TGeoTranslation(0,0,64.0+(222.7-64.0)/2.0));
    top->AddNodeOverlap(IDS_side_parts,1,new TGeoTranslation(0,0,-(64.0+(222.7-64.0)/2.0)));
    top->AddNodeOverlap(IDS_connection_parts_R,1,new TGeoTranslation(0,0,56.0+(64.0-56.0)/2.0));
    top->AddNodeOverlap(IDS_connection_parts_L,1,new TGeoTranslation(0,0,-(56.0+(64.0-56.0)/2.0)));

    top->AddNodeOverlap(beampipe_central_part,1,new TGeoTranslation(0,0,0));
    top->AddNodeOverlap(beampipe_side_parts,1,new TGeoTranslation(0,0,191.5+100.0));
    top->AddNodeOverlap(beampipe_side_parts,1,new TGeoTranslation(0,0,-(191.5+100.0)));
    top->AddNodeOverlap(beampipe_connection_parts_R,1,new TGeoTranslation(0,0,141.4+(191.5-141.5)/2.0));
    top->AddNodeOverlap(beampipe_connection_parts_L,1,new TGeoTranslation(0,0,-(141.4+(191.5-141.5)/2.0)));

    top->AddNodeOverlap(Pixel_support,1,new TGeoTranslation(0,0,0));

    top->DrawClone("ogl");



    const Int_t n_TPC_points = 50;
    TPolyLine3D   *TPC_endcaps[4];
    TPolyLine3D   *TPC_tube[4];
    TPolyLine3D   *TPC_tube_lines[n_TPC_points+1];

    Float_t radius_table[4] = {200,200,3.81,3.81};
    Float_t z_val_table[4]  = {200,-200,200,-200};

    Float_t radius_table_tube[4] = {50,50,50,50};
    Float_t z_val_table_tube[4]  = {200,-200,100,-100};

    for(Int_t r = 0; r < 4; r++)
    {
        TPC_endcaps[r] = new TPolyLine3D();
        Float_t radius   = radius_table[r];
        Float_t x_offset = 0.0;
        Float_t y_offset = 0.0;
        Float_t z_tpc_val   = z_val_table[r];
        for(Int_t t = 0; t < n_TPC_points+1; t++)
        {
            Float_t phi_val = ((Float_t)t/(Float_t)n_TPC_points)*(2.0*TMath::Pi());
            Float_t x_tpc_val   = radius*TMath::Cos(phi_val)+x_offset;
            Float_t y_tpc_val   = radius*TMath::Sin(phi_val)+y_offset;
            TPC_endcaps[r]->SetNextPoint(x_tpc_val,y_tpc_val,z_tpc_val);
        }
        TPC_endcaps[r]->SetLineStyle(0);
        TPC_endcaps[r]->SetLineColor(28); // 28
        TPC_endcaps[r]->SetLineWidth(2);
        TPC_endcaps[r]->DrawClone("ogl");
    }

    for(Int_t r = 0; r < 4; r++)
    {
        TPC_tube[r] = new TPolyLine3D();
        Float_t radius   = radius_table_tube[r];
        Float_t x_offset = 0.0;
        Float_t y_offset = 0.0;
        Float_t z_tpc_val   = z_val_table_tube[r];
        for(Int_t t = 0; t < n_TPC_points+1; t++)
        {
            Float_t phi_val = ((Float_t)t/(Float_t)n_TPC_points)*(2.0*TMath::Pi());
            Float_t x_tpc_val   = radius*TMath::Cos(phi_val)+x_offset;
            Float_t y_tpc_val   = radius*TMath::Sin(phi_val)+y_offset;
            TPC_tube[r]->SetNextPoint(x_tpc_val,y_tpc_val,z_tpc_val);
            if(r == 0 && (t%4 == 0))
            {
                TPC_tube_lines[t] = new TPolyLine3D();
                TPC_tube_lines[t]->SetNextPoint(x_tpc_val,y_tpc_val,z_tpc_val);
                TPC_tube_lines[t]->SetNextPoint(x_tpc_val,y_tpc_val,z_val_table_tube[r+1]);
                TPC_tube_lines[t]->SetLineStyle(0);
                TPC_tube_lines[t]->SetLineColor(28); // 28
                TPC_tube_lines[t]->SetLineWidth(1);
                //TPC_tube_lines[t]->DrawClone("ogl");
            }
        }
        TPC_tube[r]->SetLineStyle(0);
        TPC_tube[r]->SetLineColor(28); // 28
        TPC_tube[r]->SetLineWidth(2);
        TPC_tube[r]->DrawClone("ogl");
    }

    TPolyLine3D   *BeamLine;
    BeamLine       = new TPolyLine3D(2);
    BeamLine   ->SetPoint(0,0,0,-550);
    BeamLine   ->SetPoint(1,0,0,550);
    BeamLine   ->SetLineStyle(0);
    BeamLine   ->SetLineColor(4);
    BeamLine   ->SetLineWidth(2);
    BeamLine   ->DrawClone("ogl");
}
//----------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------
void Draw_Helix_3D(StPhysicalHelixD helix, Int_t line_color, Int_t line_style, Float_t line_width,
                   Float_t helix_min_radius, Float_t helix_max_radius,const Int_t n_helix_points, Float_t step_size)
{
    TPolyLine3D *helix_line;
    helix_line = new TPolyLine3D();

    StThreeVectorF helix_point;
    Float_t radius_old = helix_max_radius;
    for(Int_t t = 0; t < n_helix_points; t++)
    {
        Double_t path_length = t*step_size;
        helix_point = helix.at(path_length);
        Double_t x_val = helix_point.x();
        Double_t y_val = helix_point.y();
        Double_t z_val = helix_point.z();
        Double_t radius = TMath::Sqrt(x_val*x_val + y_val*y_val);

        if(
           (radius > helix_min_radius && radius < helix_max_radius)
          )
        {
            if((step_size < 0.0 && radius < radius_old) || step_size > 0.0)
            {
                helix_line->SetNextPoint(x_val,y_val,z_val);
                radius_old = radius;
            }
        }
    }
    helix_line->SetLineStyle(line_style);
    helix_line->SetLineColor(line_color);
    helix_line->SetLineWidth(line_width);
    helix_line->DrawClone("ogl");
}
//----------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------
void fHelixAtoPointdca(StThreeVectorF space_vec, StPhysicalHelixD helixA, Float_t &pathA, Float_t &dcaAB)
{
    // V1.1
    Float_t pA[2] = {0.0,-100.0}; // the two start values for pathB, 0.0 is the origin of the helix at the first measured point
    Float_t distarray[2];
    StThreeVectorF testA;
    for(Int_t r = 0; r < 2; r++)
    {
        testA     = helixA.at(pA[r]); // 3D-vector of helixA at path pA[r]
        distarray[r] = (testA-space_vec).mag(); // dca between helixA and helixB
        //cout << "r = " << r << ", dist = " << distarray[r] << endl;
    }

    Int_t loopcounter = 0;
    Float_t scale = 1.0;
    Float_t flip  = 1.0; // checks if the minimization direction changed
    Float_t scale_length = 100.0;
    while(fabs(scale_length) > 0.005 && loopcounter < 100) // stops when the length is too small
    {
        //cout << "n = " << loopcounter << ", pA[0] = " << pA[0]
        //    << ", pA[1] = " << pA[1] << ", d[0] = " << distarray[0]
        //    << ", d[1] = " << distarray[1] << ", flip = " << flip
        //    << ", scale_length = " << scale_length << endl;
        if(distarray[0] > distarray[1])
        {
            if(loopcounter != 0)
            {
                if(flip == 1.0) scale = 0.4; // if minimization direction changes -> go back, but only the way * 0.4
                else scale = 0.7; // go on in this direction but only by the way * 0.7
            }
            scale_length = (pA[1]-pA[0])*scale; // the next length interval
            pA[0]     = pA[1] + scale_length; // the new path
            testA     = helixA.at(pA[0]); // 3D-vector of helixA at path pA[0]
            distarray[0] = (testA-space_vec).mag(); // new dca
            flip = 1.0;
        }
        else
        {
            if(loopcounter != 0)
            {
                if(flip == -1.0) scale = 0.4;
                else scale = 0.7;
            }
            scale_length = (pA[0]-pA[1])*scale;
            pA[1]     = pA[0] + scale_length;
            testA     =  helixA.at(pA[1]); // 3D-vector of helixA at path pA[0]
            distarray[1] = (testA-space_vec).mag();
            flip = -1.0;
        }
        //cout << "pA[0] = " << pA[0] << endl;
        loopcounter++;
    }
    if(distarray[0] < distarray[1])
    {
        pathA = pA[0];
        dcaAB = distarray[0];
    }
    else
    {
        pathA = pA[1];
        dcaAB = distarray[1];
    }
    //cout << "pathA = " << pathA << ", dcaAB = " << dcaAB << endl;
}
//----------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------
Double_t calcDeterminant(StThreeVectorF& v1,StThreeVectorF& v2,StThreeVectorF& v3)
{
  // calculating the Determinant of a 3 x 3 Matrix 
  // with the column vectors [v1, v2, v3]
  // using the RULE of SARRUS
  //
  // | v1(0)   v2(0)   v3(0) |      | v1(0) v2(0) v3(0)|v1(0) v2(0)  .
  // |                       |      |  \\     \\     X   |  /     /    . 
  // |                       |      |   \\     \\   / \\  | /     /     . 
  // |                       |      |    \\     \\ /   \\ |/     /      . 
  // |                       |      |     \\     X     \\/     /       . 
  // |                       |      |      \\   / \\    /\\    /        .  
  // |                       |      |       \\ /   \\  / |\\  /         . 
  // | v1(1)   v2(1)   v3(1) |   =  | v1(1) v2(1) v3(1)|v1(1) v2(1)  .
  // |                       |      |       / \\    /\\  | /\\          . 
  // |                       |      |      /   \\  /  \\ |/  \\         . 
  // |                       |      |     /     \\/    \\/    \\        . 
  // |                       |      |    /      /\\    /\\     \\       . 
  // |                       |      |   /      /  \\  / |\\     \\      .  
  // |                       |      |  /      /    \\/  | \\     \\     . 
  // | v1(2)   v2(2)   v3(2) |      | v1(2) v2(2) v3(2)| v1(2) v2(2) .  
  //                                 /      /     /  \\     \\     \\   .
  //                                                                
  //                                -      -     -    +     +     +  .

  return ( v1(0) * v2(1) * v3(2) 
	   + v2(0) * v3(1) * v1(2) 
	   + v3(0) * v1(1) * v2(2) 
	   - v3(0) * v2(1) * v1(2) 
	   - v1(0) * v3(1) * v2(2) 
	   - v2(0) * v1(1) * v3(2)); 
}
//----------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------
StThreeVectorF calculatePointOfClosestApproach(StThreeVectorF &base1, StThreeVectorF &dir1,
								    StThreeVectorF &base2, StThreeVectorF &dir2)
{
  //  calculating point of closest approach
  //        
  //        from the equations of the straight lines of g and h 
  //        g: x1 = base1 + l * dir1 
  //        h: x2 = base2 + m * dir2 
  //        
  //        you can construct the following planes:
  //        
  //        E1: e1 = base1  +  a * dir1  +  b * (dir1 x dir2)
  //        E2: e2 = base2  +  s * dir2  +  t * (dir1 x dir2)
  //        
  //        now the intersection point of E1 with g2 = {P1} 
  //        and the intersection point of E2 with g1 = {P2}
  //        
  //        form the base points of the perpendicular to both straight lines.
  //        
  //        The point of closest approach is the middle point between P1 and P2: 
  //        
  //        vertex = (p2 - p1)/2
  // 
  //        E1 ^ g2:
  //
  //           e1 = x2
  //    -->    base1  +  a * dir1  +  b * (dir1 x dir2) = base2 + m * dir2 
  //    -->    base1 - base2 = m * dir2  -  a * dir1  -  b * (dir1 x dir2)       
  //                                          (m)
  //    -->    [ dir2, -dir1, -(dir1 x dir2)] (a) = base1 - base2        
  //                                          (b)
  //           
  //           using CRAMER's RULE you can find the solution for m (a,b, not used)
  //           
  //           using the rules for converting determinants:
  //           
  //           D12 = det [dir2, -dir1, -(dir1 x dir2)] 
  //               = det [dir2,  dir1,  (dir1 x dir2)]
  //           
  //           Dm  = det [base1 - base2, -dir1, -(dir1 x dir2)]
  //               = det [base1 - base2,  dir1,  (dir1 x dir2)]
  //  
  //            m  = Dm/D12
  //           
  //           P1: p1 = x2(m)
  //                  = base2 + Dm/D12 * dir2
  //
  //        E2 ^ g1:
  //
  //           e2 = x1
  //    -->    base2  +  s * dir2  +  t * (dir1 x dir2) = base1 + l * dir1 
  //    -->    base2 - base1 = l * dir1  -  s * dir2  -  t * (dir1 x dir2)       
  //                                          (l)
  //    -->    [ dir1, -dir2, -(dir1 x dir2)] (s) = base2 - base1        
  //                                          (t)
  //           
  //           again using CRAMER's RULE you can find the solution for m (a,b, not used)
  //           
  //           using the rules for converting determinants:
  //           
  //           D21 =  det [dir1, -dir2, -(dir1 x dir2)] 
  //               =  det [dir1,  dir2,  (dir1 x dir2)]
  //               = -det [dir2,  dir1,  (dir1 x dir2)]
  //               = -D12
  //           
  //           Dl  =  det [base2 - base1, -dir2, -(dir1 x dir2)]
  //               =  det [base2 - base1,  dir1,  (dir1 x dir2)]
  //               = -det [base1 - base2,  dir1,  (dir1 x dir2)]
  //
  //            l  =   Dl/D21
  //               = - Dl/D12
  //           
  //           P2: p2 = x1(m)
  //                  = base1 - Dl/D12 * dir1
  //           
  //           
  //           vertex = p1 + 1/2 * (p2 - p1)
  //                  = 1/2 * (p2 + p1)
  //                  = 1/2 *( (base1 + base2) +  1/D12 * ( Dm * dir2 - Dl * dir1) )
  //                      

  StThreeVectorF cross = dir1.cross(dir2); // cross product: dir1 x dir2

  // straight lines are either skew or have a cross point
	      
  StThreeVectorF diff = base1;
  diff-=base2; // Difference of two base vectors base1 - base2
		
  Double_t D;
  D =  calcDeterminant(dir2, dir1 ,cross);

  if (!(fabs(D) > 0.))
    {
      ::Warning(":calculatePointOfClosestApproach","Dirs and cross-product are lin. dependent: returning default Vertex (-20000,-20000,-20000)");
      return StThreeVectorF(-20000.,-20000.,-20000.);
    }

  Double_t Dm =  calcDeterminant(diff , dir1, cross);
  Double_t Dl = -calcDeterminant(diff , dir2, cross);

  StThreeVectorF vertex;
  StThreeVectorF dm;
  StThreeVectorF dl;

  dm = dir2;
  dm *= Dm;

  dl = dir1;
  dl *= Dl;

  vertex = dm - dl;

  vertex *= ((1.)/D);

  vertex+=base1;
  vertex+=base2;
  vertex*=0.5;

  return StThreeVectorF(vertex);
}
//----------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------
StThreeVectorF calculateCrossPoint(StThreeVectorF &base1, StThreeVectorF &dir1,
							StThreeVectorF &base2, StThreeVectorF &dir2)
{ 
  // calculating cross point 
  // taking all three equations into account solving the overdetermined set of lin. equations
  // of 
  // base1 + l * dir2 =  base1 + m * dir2 
  //
  // set of lin. equations:
  //  
  //   base1(0) + l * dir1(0) = base2(0) + m * dir2(0) 
  //   base1(1) + l * dir1(1) = base2(1) + m * dir2(1)
  //   base1(2) + l * dir1(2) = base2(2) + m * dir2(2) this line is ignored
  //
  //   written in matrix form
  //
  //        l
  //   M * |   | = base2 - base1
  //       \\ m /
  //
  //   M is a 3x2 matrix
  //     
  // to solve multiply the equation by the transposed Matrix of M from the left: M 
  //     
  //  T      /  l \\                                                               .
  // M * M * |    | = M  * (base2 - base1)
  //         \\ -m /
  // MIND THE '-' of m
  //
  //     / dir1(0) dir2(0) \\                                                      .
  //     |                 |    T   / dir1(0) dir1(1) dir1(2) \\                   .
  // M = | dir1(1) dir2(1) |,  M  = |                         |
  //     |                 |        \\ dir2(0) dir2(1) dir2(2) /                   .
  //     \\ dir1(2) dir2(2) /                                    
  //
  //  T      / (dir1(0)*dir1(0) + dir1(1)*dir1(1) + dir1(2)*dir1(2))   (dir1(0)*dir2(0) + dir1(1)*dir2(1) + dir1(2)*dir2(2))  \\ .
  // M * M = |                                                                                                                |
  //         \\ (dir1(0)*dir2(0) + dir1(1)*dir2(1) + dir1(2)*dir2(2))   (dir2(0)*dir2(0) + dir2(1)*dir2(1) + dir2(2)*dir2(2))  /                        
  //
  //  T       / d1d1 d1d2 \\                           .
  // M  * M = |           |
  //          \\ d1d2 d2d2 /
  //
  // diff = base2 - base1
  //
  //  T           /  (dir1(0)*diff(0) + dir1(1)*diff(1) + dir1(2)*diff(2)) \\         .
  // M  * diff =  |                                                        |
  //              \\  (dir2(0)*diff(0) + dir2(1)*diff(1) + dir2(2)*diff(2)) /
  //
  //  T           /  d1diff  \\                                          .
  // M  * diff =  |          |
  //              \\  d2diff  /
  // 
  // now the new Matrix set is to be solved by CRAMER'S Rule:
  // 
  // / d1d1 d1d2 \\   /  l \\   /  d1diff \\                   .
  // |           | * |    | = |          |
  // \\ d1d2 d2d2 /   \\ -m /   \\  d2diff /
  //
  //     | d1d1 d1d2 |
  // D = |           | = d1d1*d2d2 - d1d2*d1d2;
  //     | d1d2 d2d2 |
  // 
  //     | d1diff d1d2 |
  // Dl= |              | = d1diff*d2d2 - d1d2*d2diff;
  //     | d2diff d2d2 |              
  //
  // l = Dl/D = l_cross
  // 
  // vertex = base1 + l_cross * dir1
  //

  Double_t d1d1 = dir1(0)*dir1(0) + dir1(1)*dir1(1) + dir1(2)*dir1(2);
  Double_t d2d2 = dir2(0)*dir2(0) + dir2(1)*dir2(1) + dir2(2)*dir2(2);
  Double_t d1d2 = dir1(0)*dir2(0) + dir1(1)*dir2(1) + dir1(2)*dir2(2);
  
  Double_t D = d1d1*d2d2 - (d1d2*d1d2);
  
  if (!(fabs(D) > 0.))
    {
      ::Warning("calculateCrossPoint","Error while calculating cross point ... eqns are lin. dependent:returning default Vertex (-20000,-20000,-20000)");
      return StThreeVectorF(-20000.,-20000.,-20000.);
    }

  Double_t d1diff = dir1(0)*(base2(0)-base1(0))+dir1(1)*(base2(1)-base1(1))+dir1(2)*(base2(2)-base1(2));
  Double_t d2diff = dir2(0)*(base2(0)-base1(0))+dir2(1)*(base2(1)-base1(1))+dir2(2)*(base2(2)-base1(2));

  Double_t Dlambda = d1diff*d2d2-d1d2*d2diff;
  
  Double_t lambda = Dlambda/D;
  
  StThreeVectorF vertex;
  vertex += dir1;
  vertex *= lambda;
  vertex += base1;

  //cout << "Cross point calculated" << endl;
  return StThreeVectorF(vertex);

 // return StThreeVectorF(-20000.,-20000.,-20000.);
}
//----------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------
Int_t get_HFT_det_index(Int_t sensor_id, Int_t &HFT_det_index, Int_t &HFT_PXL_in_out_IST_index)
{
    // Determines detector index (see definition below) based on the sonsor id
    // 0 = inner pixel left
    // 1 = outer pixel left
    // 2 = IST left
    // 3 = inner pixel right
    // 4 = outer pixel right
    // 5 = IST right

    HFT_det_index            = -1;
    HFT_PXL_in_out_IST_index = -1;

    if(
       (sensor_id >= 1   && sensor_id <= 10)  ||
       (sensor_id >= 41  && sensor_id <= 50)  ||
       (sensor_id >= 81  && sensor_id <= 90)  ||
       (sensor_id >= 121 && sensor_id <= 130) ||
       (sensor_id >= 161 && sensor_id <= 170)
      )
    {
        HFT_det_index = 0;
    }

    if(
       (sensor_id >= 11   && sensor_id <= 40)  ||
       (sensor_id >= 51   && sensor_id <= 80)  ||
       (sensor_id >= 91   && sensor_id <= 120) ||
       (sensor_id >= 131  && sensor_id <= 160) ||
       (sensor_id >= 171  && sensor_id <= 200)
      )
    {
        HFT_det_index = 1;
    }
    if(
       sensor_id >= 1001 && sensor_id <= 1072
      )
    {
        HFT_det_index = 2;
    }
    if(
       (sensor_id >= 1+200   && sensor_id <= 10+200)  ||
       (sensor_id >= 41+200  && sensor_id <= 50+200)  ||
       (sensor_id >= 81+200  && sensor_id <= 90+200)  ||
       (sensor_id >= 121+200 && sensor_id <= 130+200) ||
       (sensor_id >= 161+200 && sensor_id <= 170+200)
      )
    {
        HFT_det_index = 3;
    }
    if(
       (sensor_id >= 11+200   && sensor_id <= 40+200)  ||
       (sensor_id >= 51+200   && sensor_id <= 80+200)  ||
       (sensor_id >= 91+200   && sensor_id <= 120+200) ||
       (sensor_id >= 131+200  && sensor_id <= 160+200) ||
       (sensor_id >= 171+200  && sensor_id <= 200+200)
      )
    {
        HFT_det_index = 4;
    }
    if(
       sensor_id >= 1073 && sensor_id <= 1145
      )
    {
        HFT_det_index = 5;
    }

    if(HFT_det_index == 0 || HFT_det_index == 3) HFT_PXL_in_out_IST_index = 0; // inner PXL
    if(HFT_det_index == 1 || HFT_det_index == 4) HFT_PXL_in_out_IST_index = 1; // outer PXL
    if(HFT_det_index == 2 || HFT_det_index == 5) HFT_PXL_in_out_IST_index = 2; // IST


    return HFT_det_index;
}
//----------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------
Double_t calculateMinimumDistanceStraightToPoint(StThreeVectorF &base, StThreeVectorF &dir,
									 StThreeVectorF &point)
{
  // calculates the minimum distance of a point to a straight given as parametric straight x = base + n * dir

  if (!(dir.mag()>0))
    {
      return -1000000.;
    }
  
  StThreeVectorF diff = base-point;

  StThreeVectorF cross = dir.cross(diff);
  
  return cross.mag()/dir.mag();
}
//----------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------
StThreeVectorF calculateDCA_vec_StraightToPoint(StThreeVectorF &base, StThreeVectorF &dir,
									 StThreeVectorF &point)
{
  // calculates the minimum distance vector of a point to a straight given as parametric straight x = base + n * dir

    StThreeVectorF diff = base-point;
    Double_t proj_val = diff.dot(dir/dir.mag());
    StThreeVectorF proj_dir = proj_val*dir/dir.mag();

    StThreeVectorF dist_vec = proj_dir - diff;

    return dist_vec;
}
//----------------------------------------------------------------------------------------



#endif /* STREADHFTTREES_H */
