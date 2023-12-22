clear all
% Constructs a multimedia reader object
vidReader1 = VideoReader('echo1.avi');
vidReader1 = VideoReader('echo2.avi');

figure

% Initialize the optical flow object
opticFlow = opticalFlowLK('NoiseThreshold', 0.0001);
while hasFrame(vidReader1)
    % Read the rgb frame
    frameRGB  = readFrame(vidReader1);
    % Convert rgb to grayscale
    frameGray =wiener2 ((rgb2gray(frameRGB)));
    %wiener2
    % Compute optical flow
    flow = estimateFlow(opticFlow, frameGray);
    % Display rgb video frame with flow vectors
    imshow(frameRGB);
    hold on;
    plot(flow, 'DecimationFactor', [5 5], 'ScaleFactor', 10);
    drawnow;
    hold off;
end