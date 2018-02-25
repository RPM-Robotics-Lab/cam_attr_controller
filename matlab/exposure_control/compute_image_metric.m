function val = compute_image_metric (t)
% val = compute_image_metric (t)
% 
%    Given time value 't' compute image metric 'val'
%    When data is available from file 
%    return corresponding val directly
%
%-----------------------------------------------------------------
%    History:
%    Date            Who         What
%    -----------     -------     -----------------------------
%    2017.08.28      ak          created and written

global time metric

if (~isemptry(time))
    val = metric(find (time == t));
end