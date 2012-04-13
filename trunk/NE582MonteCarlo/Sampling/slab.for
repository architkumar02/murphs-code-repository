      program slab
      real totxs(2),scat(2,2),bin(6),bin2(6),tbin(6)
      real sour(2),totscat(2),mean_variance,mu,mfp,mean_sd
C***********************************************************************
C                                                                      *
C       Monte Carlo calculation of a slab:                             *
C                                                                      *
C       |      |                 g                   |                 *
C       |      |         -----------------           |                 *
C       |      |         Grp 1       Grp 2           |                 *
C       |      | Total    0.1        0.2             |                 *
C       |      | G1->g    0.05       0.04            |                 *
C       |S1 4  | G2->g    0          0.1             |                 *
C       |S2 6  |                                     |                 *
C       |      |                                     |                 *
C       |      |                                     |                 *
C       |      |                                     |                 *
C       |      |                                     |                 *
C       | 5 cm |              45 cm                  |                 *
C       |<---->|<----------------------------------->|                 *
C       |      |                                     |                 *
C                                                                      *
C       Set the source and cross sections                              *
C                                                                      *
C***********************************************************************
C                                                                      *
C     Variable definitions:                                            *
C       totxs(ig)   Total cross section in group ig, 1/cm              *
C       scat(ig,jg) Scattering cross section from group ig to jg, 1/cm *
C       totscat(ig) Total scattering in group ig (i.e., sum of scat    *
C                      for all other groups jg                         *
C       sour(ig)    Source in group ig, #/cm2/sec                      *
C       bin(ib)     Bin values                                         *
C                   ib = 1 Left leakage for group 1                    *
C                      = 2 Left leakage for group 2                    *
C                      = 3 Right leakage for group 1                   *
C                      = 4 Right leakage for group 2                   *
C                      = 5 Flux for group 1                            *
C                      = 6 Flux for group 2                            *
C       ig          Current energy group of the particle               *
C       mu          Current direction cosine of the particle, (-1,1)   *
C       x           Current position of particle                       *
C       dd          Distance to next collision                         *
C       dx          x dimension distance to next collision = dd*mu     *
C       mfp         Mean free paths to next collision                  *
C                                                                      *
C***********************************************************************
      totxs(1)=0.1
      totxs(2)=0.2
      scat(1,1)=.05
      scat(1,2)=0.04
      scat(2,1)=0.
      scat(2,2)=0.1
      sour(1)=4.
      sour(2)=6.
C***********************************************************************
C                                                                      *
C       Find total scattering cross section for each group             *
C                                                                      *
C***********************************************************************
      ng=2
      do ig=1,ng
        totscat(ig)=0.
        do jg=1,ng
          totscat(ig)=totscat(ig)+scat(ig,jg)
        enddo
      enddo
C***********************************************************************
C                                                                      *
C       Convert source to CDF                                          *
C                                                                      *
C***********************************************************************
      do ig=2,ng
        sour(ig)=sour(ig-1)+sour(ig)
      enddo
      do ig=1,ng
        sour(ig)=sour(ig)/sour(ng)
      enddo
C***********************************************************************
C                                                                      *
C       Convert scattering cross sections to CDF                       *
C                                                                      *
C***********************************************************************
      do ig=1,ng
        do jg=2,ng
          scat(ig,jg)=scat(ig,jg-1)+scat(ig,jg)
        enddo
        do jg=1,ng
          scat(ig,jg)=scat(ig,jg)/scat(ig,ng)
        enddo
      enddo
C***********************************************************************
C                                                                      *
C       Empty the total bins                                           *
C                                                                      *
C***********************************************************************
      do ib=1,6
        bin(ib)=0.
        bin2(ib)=0.
      enddo
C***********************************************************************
C                                                                      *
C       For each history:                                              *
C                                                                      *
C***********************************************************************
      print*,' No. of histories?'
      read(*,*)nhist
      do ih=1,nhist
C***********************************************************************
C                                                                      *
C         Empty the history bins                                       *
C                                                                      *
C***********************************************************************
        do ib=1,6
          tbin(ib)=0.
        enddo
C***********************************************************************
C                                                                      *
C         Find the original position                                   *
C                                                                      *
C***********************************************************************
         x=5.*rand(0.)
C***********************************************************************
C                                                                      *
C         Find the original direction                                  *
C                                                                      *
C***********************************************************************
         mu=2.*rand(0.)-1.
C***********************************************************************
C                                                                      *
C         Find the original energy group                               *
C                                                                      *
C***********************************************************************
         if(rand(0.).lt.0.4)then
           ig=1
         else
           ig=2
         endif
C***********************************************************************
C                                                                      *
C         Find how many mean free paths it travels to next position    *
C                                                                      *
C***********************************************************************
888       test=1.0-rand(0.)
          if(test.le.0.)test=1.0-rand(0.)
           mfp=-alog(test)
C***********************************************************************
C                                                                      *
C           Convert mfp to distance in cm                              *
C                                                                      *
C***********************************************************************
         dd=mfp/totxs(ig)
C***********************************************************************
C                                                                      *
C           Translate into a new position                              *
C                                                                      *
C***********************************************************************
         x=x+dd*mu
C***********************************************************************
C                                                                      *
C         If particle has exited the left boundary:                    *
C                                                                      *
C***********************************************************************
         if(x.lt.0.)then
C***********************************************************************
C                                                                      *
C           Contribute to bins 1 or 2                                  *
C                                                                      *
C***********************************************************************
           tbin(ig)=tbin(ig)+1.
C***********************************************************************
C                                                                      *
C           Contribute to flux bins 5 or 6                             *
C           End the history                                            *
C                                                                      *
C***********************************************************************
           go to 20
         endif
C***********************************************************************
C                                                                      *
C         If particle has exited the right boundary                    *
C                                                                      *
C***********************************************************************
         if(x.gt.50.)then
C***********************************************************************
C                                                                      *
C           Contribute to bins 3 or 4                                  *
C                                                                      *
C***********************************************************************
           tbin(ig+2)=tbin(ig+2)+1.
C***********************************************************************
C                                                                      *
C           Contribute to flux bins 5 or 6                             *
C           End the history                                            *
C                                                                      *
C***********************************************************************
           go to 20
         endif
C***********************************************************************
C                                                                      *
C         If you got this far, must have been a collision.             *
C           Contribute to the flux bins 5 or 6                         *
C         If it was an absorption collision:                           *
C                                                                      *
C***********************************************************************
         if(rand(0.).gt.totscat(ig)/totxs(ig))then
C***********************************************************************
C                                                                      *
C           End the history                                            *
C                                                                      *
C***********************************************************************
           go to 20
         endif
C***********************************************************************
C                                                                      *
C         If it was a scattering collision:                            *
C           Pick the new direction                                     *
C                                                                      *
C***********************************************************************
         mu=2.*rand(0.)-1.
C***********************************************************************
C                                                                      *
C           Pick the new energy group                                  *
C                                                                      *
C***********************************************************************
         if(rand(0.).lt.scat(ig,1))then
           ig=1
         else
           ig=2
         endif
C***********************************************************************
C                                                                      *
C           Return to the point where we find the # of mfps travelled  *
C                                                                      *
C***********************************************************************
         go to 888
C***********************************************************************
C                                                                      *
C                                                                      *
C         If the history is over, dump the history bins into the total *
C                  bins                                                *
C                                                                      *
C***********************************************************************
   20   do ib=1,6
          bin(ib)=bin(ib)+tbin(ib)
          bin2(ib)=bin2(ib)+tbin(ib)*tbin(ib)
        enddo
      enddo
C***********************************************************************
C                                                                      *
C       When the problem is over, print the results for each bin:      *
C                                                                      *
C***********************************************************************
      do ib=1,6
        xhat=bin(ib)/float(nhist)
        x2hat=bin2(ib)/float(nhist)
        sample_variance=float(nhist)/float(nhist-1)*(x2hat-xhat*xhat)
        sample_sd=sqrt(sample_variance)
        mean_variance=sample_variance/float(nhist-1)
        mean_sd=sqrt(mean_variance)
        print*,' For bin  ',ib,' ==> ',xhat,' +/- ',mean_sd
      enddo
      stop
      end
