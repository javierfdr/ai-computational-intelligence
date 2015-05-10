%IM_PROFILE Computation of horizontal and vertical image profile
%
%	P = IM_PROFILE(A,NX,NY)
%	P = A*IM_PROFILE([],NX,NY)
%
% INPUT
%   A        Dataset with object images dataset (possibly multi-band)
%   NX       Number of bins for horizontal profile
%   NY       Number of bins for vertical profile
%
% OUTPUT
%   P        Dataset with profiles replacing images (possibly multi-band)
%
% DESCRIPTION
% Computes for the images in A a 1*(NX+NY) vector P storing the horizontal and
% vertical profile (i.e. normalized image projections) given by NX respectively
% NY points. Because of the normalization holds sum(P) = 1.
% The computation is based on a bilinear interpolation of A to an image of NX*NY
% points. Consequently is the horizontal profile (P(1:NX)) slightly dependent
% on NY and the vertical profile (P(NX+1:NX+NY)) slightly dependent on NX.
% Defaults : NY = NX, NX = 16.
%
% SEE ALSO
% DATASETS, DATAFILES

% Copyright: R.P.W. Duin, r.p.w.duin@prtools.org
% Faculty EWI, Delft University of Technology
% P.O. Box 5031, 2600 GA Delft, The Netherlands

function b = im_profile(a,nx,ny)

	prtrace(mfilename);
	
	if nargin < 3, ny = []; end
	if nargin < 2, nx = 16; end

  if nargin < 1 | isempty(a)
    b = mapping(mfilename,'fixed',{nx,ny});
    b = setname(b,'Image profile');
	elseif isa(a,'dataset') % allows datafiles too
		isobjim(a);
    b = filtim(a,mfilename,{nx,ny});
  elseif isa(a,'double') | isa(a,'dip_image') % here we have a single image
		if isa(a,'dip_image'), a = double(a); end
	  if isempty(ny)
			ny = nx;
		end
		if any([nx,ny] < 4)
			b = imresize(a,10*[ny,nx],'bilinear');
			sx = sum(reshape(sum(b,1),10,nx),1);
			sy = sum(reshape(sum(b,2)',10,ny),1);
			s = sum(sx);
			b = [sx sy]/s;
		else
			b = imresize(a,[ny,nx],'bilinear');
			s = sum(b(:));
			b = [sum(b,1) sum(b,2)']/s;
		end
	end
return
