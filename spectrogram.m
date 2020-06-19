a=[1,2,3,4];
figure(1)
for i = 1: 4;
    x = load (['cos_050Hz-8k{Set',num2str(a(i)),'}.txt']) ;
    x = x';
    subplot(2,4,i);imagesc(x);title(['cos\_050Hz-8k{Set',num2str(a(i)),'}']);
end
figure(2)
for i = 1: 4;
    x = load (['cos_050Hz-16k{Set',num2str(a(i)),'}.txt']) ;
    x = x';
    subplot(2,4,i);imagesc(x);title(['cos\_050Hz-16k{Set',num2str(a(i)),'}']);
end
figure(3)
for i = 1: 4;
    x = load (['cos_055Hz-8k{Set',num2str(a(i)),'}.txt']) ;
    x = x';
    subplot(2,4,i);imagesc(x);title(['cos\_055Hz-8k{Set',num2str(a(i)),'}']);
end
figure(4)
for i = 1: 4;
    x = load (['cos_055Hz-16k{Set',num2str(a(i)),'}.txt']) ;
    x = x';
    subplot(2,4,i);imagesc(x);title(['cos\_055Hz-16k{Set',num2str(a(i)),'}']);
end
figure(5)
for i = 1: 4;
    x = load (['cos_200Hz-8k{Set',num2str(a(i)),'}.txt']) ;
    x = x';
    subplot(2,4,i);imagesc(x);title(['cos\_200Hz-8k{Set',num2str(a(i)),'}']);
end
figure(6)
for i = 1: 4;
    x = load (['cos_200Hz-16k{Set',num2str(a(i)),'}.txt']) ;
    x = x';
    subplot(2,4,i);imagesc(x);title(['cos\_200Hz-16k{Set',num2str(a(i)),'}']);
end
figure(7)
for i = 1: 4;
    x = load (['cos_220Hz-8k{Set',num2str(a(i)),'}.txt']) ;
    x = x';
    subplot(2,4,i);imagesc(x);title(['cos\_220Hz-8k{Set',num2str(a(i)),'}']);
end
figure(8)
for i = 1: 4;
    x = load (['cos_220Hz-16k{Set',num2str(a(i)),'}.txt']) ;
    x = x';
    subplot(2,4,i);imagesc(x);title(['cos\_220Hz-16k{Set',num2str(a(i)),'}']);
end
figure(9)
for i = 1: 4;
    x = load (['vowel-8k{Set',num2str(a(i)),'}.txt']) ;
    x = x';
    subplot(2,4,i);imagesc(x);title(['vowel-8k{Set',num2str(a(i)),'}']);
end
figure(10)
for i = 1: 4;
    x = load (['vowel-16k{Set',num2str(a(i)),'}.txt']) ;
    x = x';
    subplot(2,4,i);imagesc(x);title(['vowel-16k{Set',num2str(a(i)),'}']);
end