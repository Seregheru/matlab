function [ ] = display_rec( rec, color )

   for j=1:size(rec,1)
     plot([rec(j,2),rec(j,2),rec(j,4),rec(j,4),rec(j,2)],[rec(j,1),rec(j,3),rec(j,3),rec(j,1),rec(j,1)],'Color',color,'LineStyle','-','LineWidth',4);
   end
   
end

